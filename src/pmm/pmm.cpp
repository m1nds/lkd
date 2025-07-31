#include <stdint.h>
#include <pmm.hpp>
#include <string.hpp>
#include <serial.hpp>

extern uint32_t kernel_start;
extern uint32_t kernel_end;
extern uint32_t bitmap_start;
extern uint32_t bitmap_end;

namespace pmm {

    constexpr uint32_t FRAME_SIZE = 4096;
    constexpr uint32_t FRAME_COUNT = 3000;

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

        uint8_t* bitmap_ptr = reinterpret_cast<uint8_t*>(&bitmap_start + 1);
        memset(bitmap_ptr, 0xFF, FRAME_COUNT / 8);

        this->_bm = utils::Bitmap(bitmap_ptr, FRAME_COUNT);
        this->_nbFrames = FRAME_COUNT;

        multiboot_memory_map_t* mmmt;
        for(int i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t))
        {
            multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
            if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
                this->set_unused(mmmt->addr_low, mmmt->len_low);
            }

            s.kprintf("[MEMMAP] Start Addr: %x | Length: %x | Type: %d\n",
                mmmt->addr_low, mmmt->len_low, mmmt->type);
        }

        this->set_used((uint32_t) &kernel_start, &kernel_end - &kernel_start);
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
