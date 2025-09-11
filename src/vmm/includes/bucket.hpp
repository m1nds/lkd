#ifndef BUCKET_H
#define BUCKET_H

#include <stddef.h>
#include <bitmap.hpp>

class Bucket
{
    public:
        static Bucket* allocate_bucket(size_t size);
        static int push_bucket(Bucket** buckets, size_t size);
        static int free_bucket(Bucket* b);
        static int free_empty_buckets(Bucket* b);
        static void remove_bucket(Bucket** b, Bucket* remove);

        Bucket* next;
        size_t size;
        size_t power_size;
        utils::Bitmap bitmap;
};

#endif /* ! BUCKET_H */
