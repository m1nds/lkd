#include <io_ports.hpp>

void IOPorts::outb(unsigned int port, unsigned char value) {
    asm volatile("outb %%al,%%dx" : : "d"(port), "a"(value));
}

unsigned char IOPorts::inb(unsigned int port) {
    unsigned char ret;
    asm volatile("inb %%dx,%%al" : "=a"(ret) : "d"(port));
    return ret;
}
