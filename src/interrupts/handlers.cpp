#include <serial.hpp>

struct interrupt_frame {
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
};

typedef void (*isr_type)(struct interrupt_frame *frame);

__attribute__((interrupt)) void isr0_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr1_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr2_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr3_handler(struct interrupt_frame *frame) {
    serial::Serial s{};
    s.write_str("Breakpoint!\n");
}

__attribute__((interrupt)) void isr4_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr5_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr6_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr7_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr8_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr9_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr10_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr11_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr12_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr13_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr14_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr15_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr16_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr17_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr18_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr19_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr20_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr21_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr22_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr23_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr24_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr25_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr26_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr27_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr28_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr29_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr30_handler(struct interrupt_frame *frame) {

}

__attribute__((interrupt)) void isr31_handler(struct interrupt_frame *frame) {

}

isr_type isr[32] = {
    isr0_handler,
    isr1_handler,
    isr2_handler,
    isr3_handler,
    isr4_handler,
    isr5_handler,
    isr6_handler,
    isr7_handler,
    isr8_handler,
    isr9_handler,
    isr10_handler,
    isr11_handler,
    isr12_handler,
    isr13_handler,
    isr14_handler,
    isr15_handler,
    isr16_handler,
    isr17_handler,
    isr18_handler,
    isr19_handler,
    isr20_handler,
    isr21_handler,
    isr22_handler,
    isr23_handler,
    isr24_handler,
    isr25_handler,
    isr26_handler,
    isr27_handler,
    isr28_handler,
    isr29_handler,
    isr30_handler,
    isr31_handler
};

