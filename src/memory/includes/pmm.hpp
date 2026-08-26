#pragma once

#include <stddef.h>
#include <stdint.h>

constexpr int FRAME_SIZE = 4096;
constexpr int MAX_ORDER = 10;

struct PageMetadata {
    uint8_t order;
    bool free;
};

struct FrameList {
    struct FrameList* next;
    struct FrameList* prev;
};

struct BuddyAllocatorEntry {
    struct FrameList* frames;
};

class PMM {
    public:
        static PMM& getInstance();
        int init();

        void* allocate(size_t order);
        void free(void* addr, size_t order);

        PMM(PMM const&) = delete;
        void operator=(PMM const&) = delete;

    private:
        size_t max_address();
        int init_metadata(size_t max_address);
        void add_block(size_t order, size_t base);
        struct FrameList* remove_block(size_t order);
        bool peek_block(size_t order);
        void debug_dump();

        PMM() {};
        static struct BuddyAllocatorEntry buddies[MAX_ORDER + 1];
        static struct PageMetadata* metadata;
        static size_t pages;
};
