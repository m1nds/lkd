#include <stdint.h>
#include <pmm.hpp>
#include <string.hpp>
#include <serial.hpp>

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

        multiboot_memory_map_t* mmmt;
        uint64_t total_mem = 0;

        for(int i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t))
        {
            multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
            uint64_t map_len = (((uint64_t) mmmt->len_high) << 32) | (mmmt->len_low);
            uint32_t map_addr = mmmt->addr_low;
            total_mem += map_len;

            if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
                s.kprintf("[MEMMAP] Start Addr: %x | Length: %X | Type: Free Memory\n",
                    map_addr, map_len);
            }
            else {
                s.kprintf("[MEMMAP] Start Addr: %x | Length: %X | Type: Unusable Memory\n",
                    map_addr, map_len);
            }
        }

        this->_nbFrames = total_mem / FRAME_SIZE;
        s.kprintf("[MEMMAP] Found %d physical frames.\n", this->_nbFrames);

        uint32_t kernel_end_pad = (uint32_t) &kernel_end & ~(0xFFF) + FRAME_SIZE;
        uint8_t* bitmap_ptr = reinterpret_cast<uint8_t*>(kernel_end_pad);
        this->_bm = utils::Bitmap(bitmap_ptr, this->_nbFrames);

        memset(bitmap_ptr, 0xFF, this->_nbFrames/ 8);

        for(int i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t))
        {
            multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
            uint64_t map_len = (((uint64_t) mmmt->len_high) << 32) | (mmmt->len_low);
            uint32_t map_addr = mmmt->addr_low;
            if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
                this->set_unused(map_addr, map_len);
            }
        }

        this->set_used((uint32_t) &kernel_start, kernel_end_pad - (uint32_t) &kernel_start + this->_nbFrames);
    }

    void PMM::set_used(uint32_t regionAddr, uint32_t regionSize) {
        uint32_t frameBase = regionAddr / FRAME_SIZE;
        uint32_t frames = regionSize / FRAME_SIZE;

        for (uint32_t i = 0; i < frames; i++) {
            this->_bm.set_bit(frameBase + i);
        }
    }

    void PMM::set_unused(uint32_t regionAddr, uint32_t regionSize) {
        uint32_t frameBase = regionAddr / FRAME_SIZE;
        uint32_t frames = regionSize / FRAME_SIZE;

        for (uint32_t i = 0; i < frames; i++) {
            this->_bm.clear_bit(frameBase + i);
        }
    }

    void* PMM::allocate_frame() {
        for (uint32_t i = 1; i < this->_nbFrames; i++) {
            if (!this->_bm.get_bit(i)) {
                this->_bm.set_bit(i);
                return reinterpret_cast<void*>(i * FRAME_SIZE);
            }
        }
        return nullptr;
    }

    void PMM::free_frame(void* addr) {
        if (addr == nullptr) {
            return;
        }

        uint32_t value = reinterpret_cast<uint32_t>(addr);
        uint32_t bit = value / FRAME_SIZE;

        this->_bm.clear_bit(bit);
    }
}
