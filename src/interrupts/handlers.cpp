#include <serial.hpp>
#include <utils/interrupt_state.hpp>

extern "C" void isr_handler(struct interrupt_state state) {
    serial::Serial s{};
    s.kprintf("Interrupt %d\n", state.interrupt);
}

extern "C" void irq_handler(struct interrupt_state state) {

}
