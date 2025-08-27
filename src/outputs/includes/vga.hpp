#ifndef VGA_HPP
#define VGA_HPP

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

namespace vga {

    enum vga_color {
        VGA_COLOR_BLACK = 0,
        VGA_COLOR_BLUE = 1,
        VGA_COLOR_GREEN = 2,
        VGA_COLOR_CYAN = 3,
        VGA_COLOR_RED = 4,
        VGA_COLOR_MAGENTA = 5,
        VGA_COLOR_BROWN = 6,
        VGA_COLOR_LIGHT_GREY = 7,
        VGA_COLOR_DARK_GREY = 8,
        VGA_COLOR_LIGHT_BLUE = 9,
        VGA_COLOR_LIGHT_GREEN = 10,
        VGA_COLOR_LIGHT_CYAN = 11,
        VGA_COLOR_LIGHT_RED = 12,
        VGA_COLOR_LIGHT_MAGENTA = 13,
        VGA_COLOR_LIGHT_BROWN = 14,
        VGA_COLOR_WHITE = 15,
    };

    class VGA {
        public:
            void init_vga();
            ssize_t write_str(const char *buf);
            ssize_t write(const char *buf, size_t count);
            void kprintf(const char *format, ...);
            void set_color(enum vga_color fg, enum vga_color bg);
            VGA() {};

        private:
            void insert_char(unsigned char ch, uint32_t x, uint32_t y);
            void clear_screen();
            void scroll_screen();
            void write_char(unsigned char ch);
            void print_dec(uint32_t value);
            void print_hex(uint64_t value, uint32_t bits);

            static bool _init;
            static uint8_t _color;
            static uint32_t _x;
            static uint32_t _y;
            static uint32_t _width;
            static uint32_t _height;
            static uint16_t* _screen;
    };
}

#endif /* SERIAL_HPP */
