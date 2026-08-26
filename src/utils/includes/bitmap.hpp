#pragma once

#include <stdint.h>
#include <stddef.h>

class Bitmap {
    public:
        Bitmap(uint8_t* bitmap, size_t size);
        int get(uint32_t i);
        void set(uint32_t i);
        void clear(uint32_t i);
        int set_first_free();

    private:
        uint8_t* bitmap;
        size_t length;
};
