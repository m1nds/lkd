#include <utils/bitmap.hpp>

#include <stddef.h>
#include <stdint.h>

namespace utils {
    Bitmap::Bitmap(uint8_t* bitmap, size_t size) {
        this->_bitmap = bitmap;
        this->_size = size;
    }

    bool Bitmap::get_bit(uint32_t i) {
        uint32_t bitmap_index = i / 8;
        uint32_t bitmap_offset = i % 8;

        if (this->_size >= bitmap_index) {
            return false;
        }

        if (((this->_bitmap[bitmap_index] >> bitmap_offset) & 1) == 0) {
            return false;
        }

        return true;
    }

    bool Bitmap::set_bit(uint32_t i) {
        uint32_t bitmap_index = i / 8;
        uint32_t bitmap_offset = i % 8;

        if (this->_size >= bitmap_index) {
            return false;
        }

        this->_bitmap[bitmap_index] |= ((1 << 7) >> bitmap_offset);

        return true;
    }

    bool Bitmap::clear_bit(uint32_t i) {
        uint32_t bitmap_index = i / 8;
        uint32_t bitmap_offset = i % 8;

        if (this->_size >= bitmap_index) {
            return false;
        }

        this->_bitmap[bitmap_index] &= ~((1 << 7) >> bitmap_offset);

        return true;
    }
}