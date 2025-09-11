#include <bitmap.hpp>

#include <stddef.h>
#include <stdint.h>

namespace utils {
    Bitmap::Bitmap(uint8_t* bitmap, size_t size) {
        this->_bitmap = bitmap;
        this->_size = size / 8;
    }

    size_t Bitmap::size() const {
        return this->_size;
    }
    const uint8_t* Bitmap::ptr() const {
        return this->_bitmap;
    }

    int Bitmap::set_first_free() {
        for (size_t i = 0; i < this->_size; i++) {
            if (! this->get_bit(i)) {
                this->set_bit(i);
                return i;
            }
        }
        return -1;
    }

    bool Bitmap::is_empty() const {
        for (size_t i = 0; i < this->_size; i++) {
            if (this->_bitmap[i] != 0) {
                return false;
            }
        }
        return true;
    }

    bool Bitmap::get_bit(uint32_t i) {
        uint32_t bitmap_index = i / 8;
        uint32_t bitmap_offset = i % 8;

        if (bitmap_index >= this->_size) {
            return false;
        }

        return this->_bitmap[bitmap_index] & (1 << bitmap_offset);
    }

    bool Bitmap::set_bit(uint32_t i) {
        uint32_t bitmap_index = i / 8;
        uint32_t bitmap_offset = i % 8;

        if (bitmap_index >= this->_size) {
            return false;
        }

        this->_bitmap[bitmap_index] |= (1 << bitmap_offset);
        return true;
    }

    bool Bitmap::clear_bit(uint32_t i) {
        uint32_t bitmap_index = i / 8;
        uint32_t bitmap_offset = i % 8;

        if (bitmap_index >= this->_size) {
            return false;
        }

        this->_bitmap[bitmap_index] &= ~(1 << bitmap_offset);
        return true;
    }
}
