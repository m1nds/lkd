#include <stdint.h>
#include <pmm.hpp>
#include <string.hpp>
#include <utils.hpp>
#include <serial.hpp>
#include <paging.hpp>

extern uint32_t kernel_start;
extern uint32_t kernel_end;

namespace pmm {

    constexpr uint32_t FRAME_SIZE = 4096;

    PMM& PMM::getInstance() {
        static PMM instance;
        return instance;
    }

    void PMM::init(multiboot_info_t* mbd) {
        serial::Serial s{};

        if (!(mbd->flags >> 6 & 0x1)) {
            s.write_str("[ERROR] INVALID MULTIBOOT FLAG in Physical Memory Manager!\n");
            return;
        }

        s.kprintf("[PMM] Kernel start: %x | Kernel_end %x\n", &kernel_start, &kernel_end);

        uint64_t total_mem = 0;

        for (uint32_t i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
            multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (P2V(mbd->mmap_addr) + i);
            uint64_t map_len = (((uint64_t) mmmt->len_high) << 32) | (mmmt->len_low);
            uint32_t map_addr = mmmt->addr_low;
            total_mem += map_len;

            if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
                s.kprintf("[PMM] Start Addr: %x | Length: %X | Type: Free Memory\n",
                          map_addr, map_len);
            } else {
                s.kprintf("[PMM] Start Addr: %x | Length: %X | Type: Unusable Memory\n",
                          map_addr, map_len);
            }
        }

        this->_nbFrames = total_mem / FRAME_SIZE;
        s.kprintf("[PMM] Found %d physical frames.\n", this->_nbFrames);

        uint32_t kernel_end_page = ((uint32_t) &kernel_end + FRAME_SIZE - 1) & ~(FRAME_SIZE - 1);
        uint32_t bitmap_size_bytes = (_nbFrames + 7) / 8;
        uint32_t bitmap_pages = (bitmap_size_bytes + FRAME_SIZE - 1) / FRAME_SIZE;
        uint8_t* bitmap_ptr = (uint8_t*) kernel_end_page;

        vmm::Page& pd = vmm::Page::get_current_pd();

        uint32_t to_map = reinterpret_cast<uint32_t>(bitmap_ptr);
        for (int i = 0; i < bitmap_pages; i++) {
            pd.map_page(V2P(to_map), to_map, 0x3);
            to_map += 0x1000;
        }

        memset(bitmap_ptr, 0xFF, bitmap_size_bytes);
        this->_bm = utils::Bitmap(bitmap_ptr, this->_nbFrames);

         s.kprintf("[PMM] Bitmap initialized at %x, size: %d bytes (%d pages)\n",
                  (uint32_t) bitmap_ptr, bitmap_size_bytes, bitmap_pages);

        for (uint32_t i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
            multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (P2V(mbd->mmap_addr) + i);
            uint64_t map_len = (((uint64_t) mmmt->len_high) << 32) | (mmmt->len_low);
            uint32_t map_addr = mmmt->addr_low;

            if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
                uint32_t start = map_addr;
                uint32_t end = map_addr + map_len;

                uint32_t kernel_start_addr = (uint32_t) &kernel_start;
                uint32_t bitmap_end_addr = (uint32_t)bitmap_ptr + bitmap_pages * FRAME_SIZE;

                if (start < kernel_start_addr) {
                    set_unused(start, kernel_start_addr - start);
                }

                if (end > bitmap_end_addr) {
                    set_unused(bitmap_end_addr, end - bitmap_end_addr);
                }
            }
        }

        set_used((uint32_t)&kernel_start, ((uint32_t)&kernel_end - (uint32_t)&kernel_start));
        set_used((uint32_t)bitmap_ptr, bitmap_pages * FRAME_SIZE);
    }

    void PMM::set_used(uint32_t regionAddr, uint32_t regionSize) {
        uint32_t frameBase = regionAddr / FRAME_SIZE;
        uint32_t frames = (regionSize + FRAME_SIZE - 1) / FRAME_SIZE;

        for (uint32_t i = 0; i < frames; i++) {
            this->_bm.set_bit(frameBase + i);
        }
    }

    void PMM::set_unused(uint32_t regionAddr, uint32_t regionSize) {
        uint32_t frameBase = regionAddr / FRAME_SIZE;
        uint32_t frames = (regionSize + FRAME_SIZE - 1) / FRAME_SIZE;

        for (uint32_t i = 0; i < frames; i++) {
            this->_bm.clear_bit(frameBase + i);
        }
    }

    void* PMM::allocate_frame() {
        for (uint32_t i = 1; i < this->_nbFrames; i++) {
            if (!this->_bm.get_bit(i)) {
                this->_bm.set_bit(i);
                return reinterpret_cast<void*>(P2V(i * FRAME_SIZE));
            }
        }
        return nullptr;
    }

    void PMM::free_frame(void* addr) {
        if (addr == nullptr) return;

        uint32_t frame = reinterpret_cast<uint32_t>(V2P(addr)) / FRAME_SIZE;
        if (frame >= _nbFrames) return;

        this->_bm.clear_bit(frame);
    }

}

