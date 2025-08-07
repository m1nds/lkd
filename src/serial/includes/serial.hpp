#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define COM1 0x3F8

namespace serial {
    class Serial {
        public:
            void init_serial();
            ssize_t write_str(const char *buf);
            ssize_t write(const char *buf, size_t count);
            void kprintf(const char *format, ...);
            Serial() {};
        private:
            int is_transmit_empty();
            void write_char(char ch);
            void print_dec(uint32_t value);
            void print_hex(uint64_t value, uint32_t bits);

            static bool _init;
    };
}

#endif /* SERIAL_HPP */
