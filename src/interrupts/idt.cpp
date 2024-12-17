#include <idt.hpp>
#include <stdint.h>

extern "C" {
    extern void *isr[32];
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
    IdtEntry idt[256];

    struct {
        uint16_t limit;
        uint32_t offset;
    } __attribute__((packed)) idtr =
    {
        .limit = sizeof(IdtEntry) * IDT_ENTRIES - 1,
        .offset = reinterpret_cast<uint32_t>(idt)
    };

    void idt_init() {
        for (uint32_t i = 0; i < 32; i++) {
            idt[i].idt_setup_entry(reinterpret_cast<uint32_t>(isr[i]), 0x08, 0x8e);
        }

        for (uint32_t i = 32; i < IDT_ENTRIES; i++) {
            idt[i].idt_setup_entry(0, 0, 0);
        }

        __asm__ volatile("lidt %0" : : "m"(idtr));
        __asm__ volatile("sti");
    }
}
