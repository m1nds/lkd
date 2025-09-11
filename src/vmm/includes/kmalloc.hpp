#ifndef KMALLOC_HPP
#define KMALLOC_HPP

#include <stddef.h>

class kmalloc {
    private:

    public:
        static void* malloc(size_t size);
        static void* calloc(size_t nmemb, size_t size);
        static void* realloc(void* ptr, size_t size);
        static void free(void* ptr);
};

#endif /* KMALLOC_HPP */
