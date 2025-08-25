#include <idt.hpp>
#include <pic.hpp>
#include <serial.hpp>

#include <stdint.h>
#include <string.h>

extern "C" {
    extern void *isr_table[32];
    extern void *irq_table[16];
    extern void *isr_128;
}

namespace idt {

    IdtEntry::IdtEntry(uint32_t offset, uint16_t selector, uint8_t flags) {
        this->idt_setup_entry(offset, selector, flags);
    }

    void IdtEntry::idt_setup_entry(uint32_t offset, uint16_t selector, uint8_t flags) {
        this->_offset_low = offset & 0xFFFF;
        this->_selector = selector;
        this->_reserved = 0;
        this->_flags = flags;
        this->_offset_high = (offset >> 16) & 0xFFFF;
    }

    constexpr unsigned int IDT_ENTRIES = 256;
    __attribute__((aligned(0x10))) IdtEntry idt[256];

    struct {
        uint16_t limit;
        uint32_t offset;
    } __attribute__((packed)) idtr =
    {
        .limit = sizeof(IdtEntry) * IDT_ENTRIES - 1,
        .offset = reinterpret_cast<uint32_t>(idt)
    };

    void idt_init() {
        serial::Serial s{};

        memset(&idt, 0, sizeof(idt));

        for (uint32_t i = 0; i < 32; i++) {
            idt[i].idt_setup_entry(reinterpret_cast<uint32_t>(isr_table[i]), 0x08, 0x8e);
        }

        idt[0x80].idt_setup_entry(reinterpret_cast<uint32_t>(&isr_128), 0x08, 0x8e);

        pic::PIC pic{};
        s.write_str("[MAIN] PIC > OK\n");

        for (uint32_t i = 32; i < 48; i++) {
            idt[i].idt_setup_entry(reinterpret_cast<uint32_t>(irq_table[i - 32]), 0x08, 0x8e);
        }

        __asm__ volatile("lidt %0" : : "m"(idtr));
        __asm__ volatile("sti");
    }
}
