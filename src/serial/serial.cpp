#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/types.h>

#include <serial.hpp>
#include <io.hpp>

namespace serial {

    bool Serial::_init = false;

    // Memory for kprintf
    uint32_t stack[256];
    size_t stack_idx = 0;

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

    void Serial::write_char(char ch) {
        if (!_init) {
            this->init_serial();
            _init = true;
        }

        while (this->is_transmit_empty() == 0);

        io::outb(COM1, ch);
    }

    ssize_t Serial::write_str(const char *buf) {
        ssize_t i = 0;
        while (buf[i]) {
            this->write_char(buf[i++]);
        }

        return i;
    }

    ssize_t Serial::write(const char *buf, size_t count) {
        for (size_t i = 0; i < count; i++) {
            this->write_char(buf[i]);
        }

        return count;
    }

    void Serial::print_hex(uint64_t value, uint32_t bits)
    {
        uint32_t digits = 0;
        static char hex[] = "0123456789ABCDEF";

        while (value)
        {
            stack[stack_idx++] = value % 16;
            digits++;
            value /= 16;
        }

        this->write_str("0x");

        uint32_t len = bits / 4;
        for (uint8_t i = digits; i < len; i++) {
            this->write_char('0');
        }

        for (uint8_t i = 0; i < digits; i++)
        {
            this->write_char(hex[stack[--stack_idx]]);
        }
    }

    void Serial::print_dec(uint32_t value)
    {
        uint32_t digits = 0;

        while (value)
        {
            stack[stack_idx++] = value % 10;
            digits++;
            value /= 10;
        }

        for (uint8_t i = 0; i < digits; i++)
        {
            this->write_char('0' + stack[--stack_idx]);
        }
    }

    void Serial::kprintf(const char *format, ...) {
        va_list args;
        va_start(args, format);

        size_t i = 0;
        while (format[i]) {
            if (format[i] != '%') {
                this->write_char(format[i]);
            } else if (format[i + 1] == 'd') {
                this->print_dec(va_arg(args, uint32_t));
                i++;
            } else if (format[i + 1] == 'x') {
                this->print_hex(va_arg(args, uint32_t), 32);
                i++;
            } else if (format[i + 1] == 'X') {
                this->print_hex(va_arg(args, uint64_t), 64);
                i++;
            } else {
                this->write_str("[WARNING] Invalid printing format!\n");
            }
            i++;
        }

        va_end(args);
    }
}
