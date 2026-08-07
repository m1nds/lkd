#include <io_ports.hpp>
#include <uart_method.hpp>

constexpr int COM1 = 0x3F8;

void ArchUART::init() {
    IOPorts::outb(COM1 + 1, 0x00);
    IOPorts::outb(COM1 + 3, 0x80);
    IOPorts::outb(COM1 + 0, 0x03);
    IOPorts::outb(COM1 + 1, 0x00);
    IOPorts::outb(COM1 + 3, 0x03);
    IOPorts::outb(COM1 + 2, 0xC7);
    IOPorts::outb(COM1 + 4, 0x0B);
    IOPorts::outb(COM1 + 4, 0x1E);
    IOPorts::outb(COM1 + 0, 0xAE);
    IOPorts::outb(COM1 + 4, 0x0F);
}

int ArchUART::is_transmit_empty() { return IOPorts::inb(COM1 + 5) & 0x20; }

void ArchUART::write_char(unsigned char ch) {
    while (this->is_transmit_empty() == 0)
        ;
    IOPorts::outb(COM1, ch);
}
