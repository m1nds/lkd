#ifndef IDT_H
#define IDT_H

#include <stdint.h>
namespace idt {
    class IdtEntry {
        public:
            IdtEntry() {}
            IdtEntry(uint32_t offset, uint16_t selector, uint8_t flags);
            void idt_setup_entry(uint32_t offset, uint16_t selector, uint8_t flags);
        private:
            uint16_t _offset_low;
            uint16_t _selector;
            uint8_t _reserved;
            uint8_t _flags;
            uint16_t _offset_high;
    } __attribute__((packed));

    void idt_init();
};

#endif /* ! IDT_H */
