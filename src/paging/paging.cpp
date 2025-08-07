#include <stdint.h>
#include <stddef.h>
#include <new>

#include <paging.hpp>
#include <serial.hpp>
#include <pmm.hpp>

using namespace pmm;

namespace vmm {
    constexpr uint32_t PAGING_FLAG = 0x80000000;

    PageEntry::PageEntry(uint32_t value, uint16_t flags) {
        this->_value = (value & ~(0xFFF)) | (flags & 0xFFF);
    };

    void PageEntry::set_value(uint32_t value) {
        this->_value |= (value & ~(0xFFF));
    }

    void PageEntry::set_flags(uint16_t flags) {
        this->_value |= (flags & 0xFFF);
    }

    PageEntry& Page::operator[](size_t index) {
        return this->_entries[index];
    }

    const PageEntry& Page::operator[](size_t index) const {
        return this->_entries[index];
    }

    uint32_t Page::address() const {
        return reinterpret_cast<uint32_t>(this);
    }

    void kernel_init_paging() {
        serial::Serial s{};

        void* page_directory_addr = PMM::getInstance().allocate_frame();
        void* page_table_addr = PMM::getInstance().allocate_frame();

        Page& page_directory = *(new (page_directory_addr) Page{});
        Page& page_table = *(new (page_table_addr) Page{});

        page_directory[0] = PageEntry(page_table.address(), 0x3);
        for (size_t i = 1; i < 1024; i++) {
            page_directory[i] = PageEntry(0, 0x2);
        }

        for (size_t i = 0; i < 1024; i++) {
            page_table[i] = PageEntry(i * 0x1000, 0x3);
        }

        __asm__ volatile (
            "mov %0, %%cr3 \n"
            "mov %%cr0, %%eax \n"
            "or  %1, %%eax \n"
            "mov %%eax, %%cr0 \n"
            :
            : "r"(page_directory.address()), "i"(PAGING_FLAG)
            : "eax"
        );

    }
};
