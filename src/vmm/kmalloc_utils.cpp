#include <stddef.h>
#include <stdint.h>
#include <bucket.hpp>

size_t compute_power(size_t bytes)
{
    size_t out = 16;
    while (out < bytes)
    {
        out <<= 1;
    }
    return out;
}

size_t align(size_t size, size_t align)
{
    size_t off = size % align;

    if (off == 0)
    {
        return size;
    }

    unsigned long long int out;
    if (__builtin_uaddll_overflow(size, align - off, &out))
    {
        return 0;
    }

    return out;
}

void *bitmap_offset(Bucket* b, int i)
{
    size_t value = (size_t)b->bitmap.ptr() + ((b->bitmap.size() + 7) / 8);
    return (void *)(align(value, sizeof(long double)) + (i * b->power_size));
}

void *page_begin(void *ptr, size_t page_size)
{
    size_t s = page_size - 1;
    uint8_t *p = reinterpret_cast<uint8_t*>(ptr);

    void *out = p - ((size_t) p & s);

    return out;
}

int bitmap_size(size_t size, size_t power_two)
{
    size_t slots = (size - sizeof(Bucket)) / power_two;
    if (slots == 0)
    {
        return 1;
    }

    return (int) slots;
}

