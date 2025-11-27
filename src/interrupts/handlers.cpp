#include <serial.hpp>
#include <string.h>
#include <timer.hpp>
#include <keyboard.hpp>
#include <process.hpp>
#include <pic.hpp>
#include <syscalls.hpp>
#include <interrupt_state.hpp>

extern "C" void isr_handler(struct interrupt_state* state) {
    serial::Serial s{};

    uint32_t int_num = state->interrupt_number;
    if (int_num == 0x80) {
        s.kprintf("[SYSCALLS] Triggerring a syscall, %%eax = %d\n", state->eax);
        syscalls::syscall_handler(state);
    } else if (int_num == 13) {
        s.kprintf("[ERROR] General Protection fault\n");
        s.kprintf("[ERROR] %%eip = %x\n", state->eip);
        s.kprintf("[ERROR] %%ebp = %x\n", state->ebp);
        for (;;);
    } else if (int_num == 14) {
        s.kprintf("[ERROR] Page fault\n");
        s.kprintf("[ERROR] %%eip = %x\n", state->eip);
        s.kprintf("[ERROR] %%ebp = %x\n", state->ebp);
        for (;;);
    }
}

extern "C" void irq_handler(struct interrupt_state* state) {
    uint32_t int_num = state->interrupt_number;
    switch (int_num) {
        case 0:
            timer::PIT::pit_interrupt_handler();
            user::Process::switch_process(state);
            break;
        case 1:
            keyboard::Keyboard::keyboard_interrupt_handler();
            break;
        default:
            break;
    }

    pic::PIC::pic_send_eoi(int_num);
}
