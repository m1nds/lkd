#include <serial.hpp>
#include <timer.hpp>
#include <keyboard.hpp>
#include <pic.hpp>
#include <interrupt_state.hpp>

extern "C" void isr_handler(struct interrupt_state state) {
    serial::Serial s{};

    uint32_t int_num = state.interrupt_number;
    s.kprintf("ISR int: %d\n", int_num);

}

extern "C" void irq_handler(struct interrupt_state state) {
    uint32_t int_num = state.interrupt_number;
    switch (int_num) {
        case 0:
            timer::PIT::pit_interrupt_handler();
            break;
        case 1:
            keyboard::Keyboard::keyboard_interrupt_handler();
            break;
        default:
            break;
    }

    pic::PIC::pic_send_eoi(int_num);
}
