#include <pmm.hpp>

#include <utils.hpp>
#include <uart.hpp>
#include <string.hpp>
#include <limine.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST_ID,
    .revision = 0
};

BuddyAllocatorEntry PMM::buddies[MAX_ORDER + 1] = {};
PageMetadata* PMM::metadata = nullptr;
size_t PMM::pages = 0;

PMM& PMM::getInstance() {
    static PMM instance;
    return instance;
}

void PMM::add_block(size_t order, size_t base) {
    auto hhdm_offset = hhdm_request.response->offset;
    auto framelist = reinterpret_cast<FrameList*>(base + hhdm_offset);
    framelist->prev = nullptr;

    auto& page = PMM::metadata[base / FRAME_SIZE];
    page.free = true;
    page.order = order;

    auto head = PMM::buddies[order].frames;
    if (head != nullptr) {
        head->prev = framelist;
    }

    framelist->next = head;
    PMM::buddies[order].frames = framelist;
}

FrameList* PMM::remove_block(size_t order) {
    auto hhdm_offset = hhdm_request.response->offset;
    auto head = PMM::buddies[order].frames;
    if (head == nullptr) {
        return nullptr;
    }

    auto base = reinterpret_cast<size_t>(head) - hhdm_offset;
    auto& page = PMM::metadata[base / FRAME_SIZE];
    page.free = false;

    auto next = head->next;
    if (next != nullptr) {
        next->prev = nullptr;
    }

    PMM::buddies[order].frames = next;
    return head;
}

bool PMM::peek_block(size_t order) {
    return PMM::buddies[order].frames != nullptr;
}

void* PMM::allocate(size_t order) {
    auto curr_order = order;
    while (curr_order <= MAX_ORDER && !PMM::peek_block(curr_order)) {
        curr_order++;
    }

    if (curr_order > MAX_ORDER) {
        return nullptr;
    }

    auto hhdm_offset = hhdm_request.response->offset;
    auto curr_block = PMM::remove_block(curr_order);
    auto base = reinterpret_cast<size_t>(curr_block) - hhdm_offset;

    while (curr_order > order) {
        curr_order--;
        size_t split_size = (1UL << curr_order) * FRAME_SIZE;
        PMM::add_block(curr_order, base + split_size);
    }

    PMM::metadata[base / FRAME_SIZE].order = order;
    return reinterpret_cast<void*>(base);
}

void PMM::free(void* addr, size_t order) {
    auto hhdm_offset = hhdm_request.response->offset;
    auto addr_c = reinterpret_cast<uintptr_t>(addr);

    if (addr_c / FRAME_SIZE >= PMM::pages) {
        return;
    }

    auto& curr_page = PMM::metadata[addr_c / FRAME_SIZE];
    if (curr_page.free) {
        return;
    }

    auto curr = reinterpret_cast<FrameList*>(addr_c + hhdm_offset);
    while (order < MAX_ORDER) {
        auto size = (1UL << order) * FRAME_SIZE;
        auto buddy_address = addr_c ^ size;

        if (buddy_address / FRAME_SIZE >= PMM::pages) {
            break;
        }

        auto& buddy_page = PMM::metadata[buddy_address / FRAME_SIZE];

        if (!buddy_page.free || buddy_page.order != order) {
            break;
        }

        auto buddy = reinterpret_cast<FrameList*>(buddy_address + hhdm_offset);
        auto prev = buddy->prev;
        auto next = buddy->next;

        if (prev != nullptr) {
            prev->next = next;
        } else {
            PMM::buddies[order].frames = next;
        }

        if (next != nullptr) {
            next->prev = prev;
        }

        buddy_page.free = false;
        addr_c &= ~size;
        order++;
    }

    PMM::add_block(order, addr_c);
}

void PMM::debug_dump() {
    for (size_t i = 0; i <= MAX_ORDER; i++) {
        auto curr_buddy = PMM::buddies[i];
        UARTG::kprintf("=== BuddyAllocatorEntry ===\n");
        UARTG::kprintf("Order: %d\n", i);
        UARTG::kprintf("Entries:\n");

        auto curr_framelist = curr_buddy.frames;
        while (curr_framelist != nullptr) {
            UARTG::kprintf("   - FrameList(next=%x)\n", curr_framelist->next);
            curr_framelist = curr_framelist->next;
        }
    }
}

size_t PMM::max_address() {
    auto count = memmap_request.response->entry_count;
    size_t max_address = 0;

    for (size_t i = 0; i < count; i++) {
        auto mementry = memmap_request.response->entries[i];
        auto curr_addr = mementry->base + mementry->length;
        if (curr_addr > max_address) {
            max_address = curr_addr;
        }
    }

    return max_address;
}

int PMM::init_metadata(size_t max_address) {
    size_t count = memmap_request.response->entry_count;
    size_t pages = max_address / FRAME_SIZE;
    size_t metadata_size = pages * sizeof(struct PageMetadata);

    limine_memmap_entry* save = nullptr;
    for (size_t i = 0; i < count; i++) {
        auto mementry = memmap_request.response->entries[i];
        if (mementry->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }

        if (mementry->length < metadata_size) {
            continue;
        }

        if (mementry->length == metadata_size) {
            save = mementry;
            break;
        }

        if (mementry->base == 0) {
            mementry->base += FRAME_SIZE;
            mementry->length -= FRAME_SIZE;
        }

        if (save == nullptr || (mementry->length >= metadata_size && mementry->length < save->length)) {
            save = mementry;
        }
    }

    if (save == nullptr) {
        UARTG::kprintf("Cannot find memory for page metadata!\n");
        UARTG::kprintf("Metadata size = %X\n", metadata_size);
        return -1;
    }

    auto hhdm_offset = hhdm_request.response->offset;
    PMM::pages = pages;
    PMM::metadata = reinterpret_cast<PageMetadata*>(save->base + hhdm_offset);

    memset(PMM::metadata, 0, pages * sizeof(PageMetadata));

    size_t rounded = (metadata_size % FRAME_SIZE == 0) ? metadata_size : (metadata_size & ~(FRAME_SIZE - 1)) + FRAME_SIZE;
    save->base += rounded;
    save->length -= rounded;

    if constexpr (DEBUG) {
        UARTG::kprintf("PageMetadata(addr=%X, pages=%X)\n", PMM::metadata, PMM::pages);
    }

    return 0;
}

int PMM::init() {
    memset(PMM::buddies, 0, (MAX_ORDER + 1) * sizeof(BuddyAllocatorEntry));

    size_t count = memmap_request.response->entry_count;
    size_t max_address = PMM::max_address();

    if (PMM::init_metadata(max_address) < 0) {
        return -1;
    }

    auto hhdm_offset = hhdm_request.response->offset;
    for (size_t i = 0; i < count; i++) {
        auto mementry = memmap_request.response->entries[i];
        if (mementry->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }

        if (mementry->base == 0 && mementry->length <= FRAME_SIZE) {
            continue;
        }

        if (mementry->base == 0) {
            mementry->base += FRAME_SIZE;
            mementry->length -= FRAME_SIZE;
        }

        auto base = mementry->base;
        auto length = mementry->length;
        ssize_t curr_order = MAX_ORDER;

        while (curr_order >= 0 && length > 0) {
            size_t order_size = (1UL << curr_order) * FRAME_SIZE;
            if (order_size <= length && (base % order_size) == 0) {

                if constexpr (DEBUG) {
                    UARTG::kprintf("Putting MMAPEntry(Base=%x,length=%x,Type=%d)\n", base, order_size, mementry->type);
                }

                PMM::add_block(curr_order, base);

                base += order_size;
                length -= order_size;

                curr_order = MAX_ORDER;
            } else {
                curr_order--;
            }
        }
    }

    if constexpr (DEBUG) {
        PMM::debug_dump();
    }

    UARTG::kprintf("PMM > OK\n");

    return 0;
}
