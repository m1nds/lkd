#include <kmalloc.hpp>
#include <bucket.hpp>
#include <kmalloc_utils.hpp>
#include <string.hpp>
#include <stdint.h>
#include <stddef.h>

static Bucket* buckets = nullptr;

void* kmalloc::malloc(size_t size)
{
    size_t power_two = compute_power(size);
    Bucket *current = buckets;

    while (current)
    {
        if (current->power_size == power_two)
        {
            int i = current->bitmap.set_first_free();
            if (i != -1)
            {
                return bitmap_offset(current, i);
            }
        }

        current = current->next;
    }

    if (Bucket::push_bucket(&buckets, power_two) == -1)
    {
        return NULL;
    }

    current = buckets;

    int i = current->bitmap.set_first_free();
    if (i != -1)
    {
        return bitmap_offset(current, i);
    }

    return nullptr;
}

void kmalloc::free(void *ptr)
{
    if (ptr == nullptr || buckets == nullptr)
    {
        return;
    }

    size_t page_size = 0x1000;
    Bucket *current = reinterpret_cast<Bucket*>(page_begin(ptr, page_size));

    if (current == nullptr)
    {
        return;
    }

    size_t bit_offset = ((uint8_t*) ptr - (((uint8_t*) current->bitmap.ptr())) +
            current->bitmap.size()) / current->power_size;

    current->bitmap.clear_bit(bit_offset);
    if (current->bitmap.is_empty())
    {
        Bucket::remove_bucket(&buckets, current);
    }
}

void* kmalloc::realloc(void *ptr, size_t size)
{
    if (size == 0)
    {
        kmalloc::free(ptr);
        return nullptr;
    }

    if (ptr == nullptr)
    {
        return kmalloc::malloc(size);
    }

    size_t page_size = 0x1000;
    Bucket *current = reinterpret_cast<Bucket*>(page_begin(ptr, page_size));

    if (current && size <= current->power_size)
    {
        return ptr;
    }

    void *copy = kmalloc::malloc(size);
    if (copy == nullptr)
    {
        return nullptr;
    }

    memcpy(copy, ptr, current->power_size);
    kmalloc::free(ptr);

    return copy;
}

void* kmalloc::calloc(size_t nmemb, size_t size)
{
    unsigned long long int result;
    if (__builtin_umulll_overflow(nmemb, size, &result))
    {
        return nullptr;
    }

    uint8_t *out = reinterpret_cast<uint8_t*>(kmalloc::malloc(result));
    if (out == nullptr)
    {
        return nullptr;
    }

    memset(out, 0, result);

    return out;
}
