#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <stdint.h>
#include <stddef.h>

namespace utils {
    class Bitmap {
        private:
            uint8_t* _bitmap;
            size_t _size;

        public:
            Bitmap(uint8_t* bitmap, size_t size);
            bool get_bit(uint32_t i);
            bool set_bit(uint32_t i);
            bool clear_bit(uint32_t i);
    };
}

#endif /* BITMAP_HPP */
