#include <pic.hpp>
#include <io.hpp>

namespace pic {

    bool PIC::_init = false;

    // IO Ports for the PICs
    constexpr uint32_t PIC_MASTER_COMMAND = 0x20;
    constexpr uint32_t PIC_SLAVE_COMMAND  = 0xA0;
    constexpr uint32_t PIC_MASTER_DATA = 0x21;
    constexpr uint32_t PIC_SLAVE_DATA  = 0xA1;

    // ICW1 Macro
    constexpr uint32_t ICW1 = 0x11;

    // Offsets for ICW2
    constexpr uint32_t PIC_MASTER_OFFSET = 0x20;
    constexpr uint32_t PIC_SLAVE_OFFSET  = 0x28;

    // ICW3 Macros
    constexpr uint32_t PIC_MASTER_ICW3 = 0x04;
    constexpr uint32_t PIC_SLAVE_ICW3 = 0x02;

    // ICW4 Macro
    constexpr uint32_t ICW4 = 0x1;

    PIC::PIC()
    {
        if (PIC::_init) {
            return;
        }

        PIC::_init = true;

        // Remaps the Programmable Interrupt Controller
        // ICW1
        io::outb(PIC_MASTER_COMMAND, ICW1);
        io::outb(PIC_SLAVE_COMMAND, ICW1);

        // ICW2
        io::outb(PIC_MASTER_DATA, PIC_MASTER_OFFSET);
        io::outb(PIC_SLAVE_DATA, PIC_SLAVE_OFFSET);

        // ICW3
        io::outb(PIC_MASTER_DATA, PIC_MASTER_ICW3);
        io::outb(PIC_SLAVE_DATA, PIC_SLAVE_ICW3);

        // ICW4
        io::outb(PIC_MASTER_DATA, ICW4);
        io::outb(PIC_SLAVE_DATA, ICW4);

        io::outb(PIC_MASTER_DATA, 0x0);
        io::outb(PIC_SLAVE_DATA, 0x0);

    }

    void PIC::pic_send_eoi(uint32_t irq)
    {
        if (irq >= 8)
            io::outb(PIC_SLAVE_COMMAND, 0x20);

        io::outb(PIC_MASTER_COMMAND, 0x20);
    }
};
