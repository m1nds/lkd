#ifndef PAGING_HPP
#define PAGING_HPP

#include <stdint.h>
#include <stddef.h>

namespace vmm {
    class PageEntry {
        public:
            PageEntry() = default;
            PageEntry(uint32_t value, uint16_t flags);
            void set_flags(uint16_t flags);
            void set_value(uint32_t value);

        private:
            uint32_t _value;
    };

    class Page {
        public:
            PageEntry& operator[](size_t index);
            const PageEntry& operator[](size_t index) const;
            uint32_t address() const;

        private:
            PageEntry _entries[1024];
    };

    void kernel_init_paging();
};

#endif
