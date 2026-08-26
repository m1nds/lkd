#include <bitmap.hpp>

#include <stddef.h>
#include <stdint.h>

constexpr int SHIFT = 3;
constexpr int MASK = 8 - 1;

#define INDEX(x) (x >> SHIFT)
#define OFFSET(x) (x & MASK)

Bitmap::Bitmap(uint8_t* ptr, size_t length) {
    this->bitmap = ptr;
    this->length = length;
}

int Bitmap::get(uint32_t i) {
    return this->bitmap[INDEX(i)] & (1 << OFFSET(i));
}

void Bitmap::set(uint32_t i) {
    this->bitmap[INDEX(i)] |= (1 << OFFSET(i));
}

void Bitmap::clear(uint32_t i) {
    this->bitmap[INDEX(i)] &= ~(1 << OFFSET(i));
}

int Bitmap::set_first_free() {
    for (size_t i = 0; i < this->length; i++) {
        if (!this->get(i)) {
            this->set(i);
            return i;
        }
    }
    return -1;
}


