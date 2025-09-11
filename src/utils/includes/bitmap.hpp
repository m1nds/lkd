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
            Bitmap() {};
            Bitmap(uint8_t* bitmap, size_t size);
            int set_first_free();
            size_t size() const;
            const uint8_t* ptr() const;
            bool is_empty() const;
            bool get_bit(uint32_t i);
            bool set_bit(uint32_t i);
            bool clear_bit(uint32_t i);
    };
}

#endif /* BITMAP_HPP */
