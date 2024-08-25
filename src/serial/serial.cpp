#include <stddef.h>
#include <sys/types.h>

#include <serial.hpp>
#include <io.hpp>

namespace serial {

    bool Serial::_init = false;

    void Serial::init_serial() {
        io::outb(COM1 + 1, 0x00);
        io::outb(COM1 + 3, 0x80);
        io::outb(COM1 + 0, 0x03);
        io::outb(COM1 + 1, 0x00);
        io::outb(COM1 + 3, 0x03);
        io::outb(COM1 + 2, 0xC7);
        io::outb(COM1 + 4, 0x0B);
        io::outb(COM1 + 4, 0x1E);
        io::outb(COM1 + 0, 0xAE);
        io::outb(COM1 + 4, 0x0F);
    }

    int Serial::is_transmit_empty() {
        return io::inb(COM1 + 5) & 0x20;
    }

    void Serial::write_serial(char ch) {
        if (!_init) {
            this->init_serial();
            _init = true;
        }

        while (this->is_transmit_empty() == 0);

        io::outb(COM1, ch);
    }

    ssize_t Serial::write(const char *buf, size_t count) {
        for (size_t i = 0; i < count; i++) {
            this->write_serial(buf[i]);
        }

        return count;
    }

}
