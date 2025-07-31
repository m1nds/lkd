#ifndef INTERRUPT_STATE_HPP
#define INTERRUPT_STATE_HPP

#include <stdint.h>

struct interrupt_state {
    uint32_t eax, ebx, ecx, ebp, edi, esi, edx;
    uint32_t interrupt, error_code, eip, cs, eflags;
} __attribute__ ((packed));

#endif
