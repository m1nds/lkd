#pragma once

#include <stdint.h>

class MMIO {
  public:
    static void mmio_write(uint32_t reg, uint32_t data);
    static uint32_t mmio_read(uint32_t reg);
    static void memory_barrier();
};
