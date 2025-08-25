#ifndef INTERRUPT_STATE_HPP
#define INTERRUPT_STATE_HPP

#include <stdint.h>

struct interrupt_state {
    uint32_t edi, esi, ebp, esp_dummy, edx, ecx, ebx, eax;
    uint32_t interrupt_number;
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;

    uint32_t user_esp;
    uint32_t user_ss;
} __attribute__((packed));

#endif
