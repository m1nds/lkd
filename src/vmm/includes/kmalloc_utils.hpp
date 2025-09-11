#ifndef KMALLOC_UTILS_H
#define KMALLOC_UTILS_H

#include <stddef.h>
#include <bucket.hpp>

size_t align(size_t size, size_t align);
size_t compute_power(size_t bytes);
void *page_begin(void *ptr, size_t page_size);
int bitmap_size(size_t size, size_t power_two);
void *bitmap_offset(Bucket *b, int i);

#endif /* ! KMALLOC_UTILS_H */
