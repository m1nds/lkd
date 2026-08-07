#include <mmio.hpp>

void MMIO::mmio_write(uint32_t reg, uint32_t data) {
    *(reinterpret_cast<volatile uint32_t*>(reg)) = data;
}

uint32_t MMIO::mmio_read(uint32_t reg) {
    return *(reinterpret_cast<volatile uint32_t*>(reg));
}

void MMIO::memory_barrier() { asm volatile("dmb sy" ::: "memory"); }
