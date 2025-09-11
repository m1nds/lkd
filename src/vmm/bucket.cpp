#include <bucket.hpp>
#include <utils.hpp>
#include <pmm.hpp>
#include <string.hpp>
#include <kmalloc_utils.hpp>

#include <stdint.h>

Bucket* Bucket::allocate_bucket(size_t size)
{
    size_t page_size = 0x1000;
    size_t new_size = align(size + sizeof(Bucket), page_size);

    Bucket *out = reinterpret_cast<Bucket*>(pmm::PMM::getInstance().allocate_frame());

    if (out == nullptr)
    {
        return nullptr;
    }

    out->power_size = size;
    out->size = new_size;
    out->next = nullptr;

    uint8_t *bm_ptr = reinterpret_cast<uint8_t*>(out) + sizeof(Bucket);
    size_t bm_size = bitmap_size(new_size, out->power_size) + 1;

    out->bitmap = utils::Bitmap(bm_ptr, bm_size);
    memset(bm_ptr, 0, bm_size / 8 + 1);

    return out;
}

int Bucket::push_bucket(Bucket **buckets, size_t size)
{
    Bucket *new_bucket = allocate_bucket(size);
    if (new_bucket == NULL)
    {
        return -1;
    }

    new_bucket->next = *buckets;
    *buckets = new_bucket;

    return 1;
}

int Bucket::free_bucket(Bucket *b)
{
    pmm::PMM::getInstance().free_frame(b);
    return 1;
}

void Bucket::remove_bucket(Bucket **b, Bucket *remove)
{
    Bucket *current = *b;
    Bucket *prev = NULL;

    if (*b == remove)
    {
        *b = (*b)->next;
    }

    while (current && current != remove)
    {
        prev = current;
        current = current->next;
    }

    if (prev)
    {
        prev->next = (current) ? current->next : NULL;
    }

    free_bucket(remove);
}
