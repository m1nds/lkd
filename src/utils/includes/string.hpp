#ifndef STRING_HPP
#define STRING_HPP

#include <stdint.h>
#include <stddef.h>

extern "C" void memcpy(void *dst, const void *src, size_t size);
extern "C" void memset(void *dst, uint8_t val, size_t size);

#endif /* STRING_HPP */
