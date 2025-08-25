#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/types.h>

#include <vga.hpp>
#include <io.hpp>

namespace vga {

    bool VGA::_init = false;
    uint32_t VGA::_width;
    uint32_t VGA::_height;
    uint32_t VGA::_x;
    uint32_t VGA::_y;
    uint16_t* VGA::_screen;
    uint8_t VGA::_color;

    // Memory for kprintf
    uint32_t stack[256];
    size_t stack_idx = 0;

    void VGA::set_color(enum vga_color fg, enum vga_color bg) {
        VGA::_color = fg | bg << 4;
    }

    void VGA::init_vga() {
        VGA::set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        VGA::_init = true;
        VGA::_width = 80;
        VGA::_height = 25;
        VGA::_x = 0;
        VGA::_y = 0;
        VGA::_screen = reinterpret_cast<uint16_t*>(0xC03FF000);
    }

    void VGA::insert_char(unsigned char ch, uint32_t x, uint32_t y) {
        VGA::_screen[VGA::_width * y + x] = (uint16_t) ch | (uint16_t) VGA::_color << 8;
    }

    void VGA::clear_screen()
    {
        for (size_t y = 0; y < VGA::_height; y++) {
            for (size_t x = 0; x < VGA::_width; x++) {
                VGA::insert_char(' ', x, y);
            }
        }
    }

    void VGA::scroll_screen()
    {
        for (size_t y = 1; y < VGA::_height; y++)
            for (size_t x = 0; x < VGA::_width; x++)
                VGA::_screen[VGA::_width * (y - 1) + x] = VGA::_screen[VGA::_width * y + x];

        for (size_t x = 0; x < VGA::_width; x++) {
            VGA::insert_char(' ', x, VGA::_height - 1);
        }

        VGA::_y--;
        VGA::_x = 0;
    }

    void VGA::write_char(unsigned char ch) {
        if (!_init) {
            this->init_vga();
            VGA::_init = true;
        }

        if (++(VGA::_x) >= VGA::_width || ch == '\n')
        {
            VGA::_x = 0;
            VGA::_y++;

            if (ch == '\n')
                return;
        }

        if (VGA::_y >= VGA::_height) {
            VGA::scroll_screen();
        }

        VGA::insert_char(ch, VGA::_x, VGA::_y);
    }

    ssize_t VGA::write_str(const char *buf) {
        ssize_t i = 0;
        while (buf[i]) {
            this->write_char(buf[i++]);
        }

        return i;
    }

    ssize_t VGA::write(const char *buf, size_t count) {
        for (size_t i = 0; i < count; i++) {
            this->write_char(buf[i]);
        }

        return count;
    }

    void VGA::print_hex(uint64_t value, uint32_t bits)
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

    void VGA::print_dec(uint32_t value)
    {
        uint32_t digits = 0;

        if (value == 0) {
            this->write_char('0');
            return;
        }

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

    void VGA::kprintf(const char *format, ...) {
        va_list args;
        va_start(args, format);

        size_t i = 0;
        while (format[i]) {
            if (format[i] != '%') {
                this->write_char(format[i]);
            } else if (format[i + 1] == '%') {
                this->write_char('%');
                i++;
            } else if (format[i + 1] == 'c') {
                this->write_char(va_arg(args, unsigned char));
                i++;
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
