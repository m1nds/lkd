#include <stdint.h>
#include <pmm.hpp>
#include <serial.hpp>

extern "C" {
    extern uint32_t kernelStart[];
    extern uint32_t kernelEnd[];
}

namespace pmm {

    constexpr uint32_t FRAME_SIZE = 4096;
    constexpr uint32_t FRAME_COUNT = 32000;

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

        uint32_t kStartAddr = reinterpret_cast<uint32_t>(&kernelStart);
        uint32_t kEndAddr = reinterpret_cast<uint32_t>(&kernelEnd);
        uint32_t  kSize = kEndAddr - kStartAddr;

        this->_bm = utils::Bitmap(reinterpret_cast<uint8_t*>(kEndAddr), FRAME_COUNT);
        this->_nbFrames = FRAME_COUNT;

        multiboot_memory_map_t* mmmt;
        for(int i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t))
        {
            mmmt = reinterpret_cast<multiboot_memory_map_t*>(mbd->mmap_addr + i);
            if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
                this->set_unused(mmmt->addr_low, mmmt->len_low);
            }

            s.kprintf("[MEMMAP] Start Addr: %x %x | Length: %x %x | Size: %x | Type: %d\n",
                mmmt->addr_high, mmmt->addr_low, mmmt->len_high, mmmt->len_low, mmmt->size, mmmt->type);
        }

        this->set_used(reinterpret_cast<uint32_t>(kStartAddr), kSize + FRAME_COUNT / 8);
    }

    void PMM::set_used(uint32_t regionAddr, uint32_t regionSize) {
        uint32_t frameBase = regionAddr / FRAME_SIZE;
        uint32_t frames = regionSize / FRAME_SIZE;

        for (uint32_t i = 0; i < frames; i++) {
            this->_bm.clear_bit(frameBase + i);
        }
    }

    void PMM::set_unused(uint32_t regionAddr, uint32_t regionSize) {
        uint32_t frameBase = regionAddr / FRAME_SIZE;
        uint32_t frames = regionSize / FRAME_SIZE;

        for (uint32_t i = 0; i < frames; i++) {
            this->_bm.set_bit(frameBase + i);
        }
    }

    void* PMM::allocate_frame() {
        for (uint32_t i = 1; i < this->_nbFrames; i++) {
            if (this->_bm.get_bit(i)) {
                this->_bm.clear_bit(i);
                return reinterpret_cast<void*>(i * FRAME_SIZE);
            }
        }
        return NULL;
    }

    void PMM::free_frame(void* addr) {
        if (addr == NULL) {
            return;
        }

        uint32_t value = reinterpret_cast<uint32_t>(addr);
        uint32_t bit = value / FRAME_SIZE;

        this->_bm.set_bit(bit);
    }
}
