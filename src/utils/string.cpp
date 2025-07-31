#include <string.hpp>

#include <stdint.h>
#include <stddef.h>

extern "C" void memcpy(void *dst, const void *src, size_t size) {
    uint8_t *c_dst = (uint8_t*) dst;
    uint8_t *c_src = (uint8_t*) src;

    for (size_t i = 0; i < size; i++) {
        c_dst[i] = c_src[i];
    }
}

extern "C" void memset(void *dst, uint8_t val, size_t size) {
    uint8_t *c_dst = (uint8_t*) dst;

    for (size_t i = 0; i < size; i++) {
        c_dst[i] = val;
    }
}
