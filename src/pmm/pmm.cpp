#include <stdint.h>
#include <pmm.hpp>
#include <serial.hpp>

extern "C" uint32_t kernelStart;
extern "C" uint32_t kernelEnd;

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

        for(int i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t))
        {
            multiboot_memory_map_t* mmmt = 
                (multiboot_memory_map_t*) (mbd->mmap_addr + i);

            s.kprintf("[MEMMAP] Start Addr: %x %x | Length: %x %x | Size: %x | Type: %d\n",
                mmmt->addr_high, mmmt->addr_low, mmmt->len_high, mmmt->len_low, mmmt->size, mmmt->type);

            if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            }
        }
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
                return (void*) (i * FRAME_SIZE);
            }
        }

        return NULL;
    }

    void PMM::free_frame(void* addr) {
        if (addr == NULL) {
            return;
        }

        uint32_t value = (uint32_t) addr;
        uint32_t bit = value / FRAME_SIZE;

        this->_bm.set_bit(bit);
    }
}
