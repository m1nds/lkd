#include <new>

#include <stddef.h>
#include <stdint.h>
#include <string.h>

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

    void task1(void)
    {
       serial::Serial s{};
       char *msg = (char*) 0x40000100; /* le message sera stocké en 0x100100 */
       msg[0] = 't';
       msg[1] = 'a';
       msg[2] = 's';
       msg[3] = 'k';
       msg[4] = '1';
       msg[5] = '\n';
       msg[6] = 0;

       s.write_str(msg);
       while(1);
       return; /* never go there */
    }

    void* get_task_pd(void* task) {
        memcpy((uint32_t*) 0x100000, task, 100);

        void* page_directory_addr = PMM::getInstance().allocate_frame();
        void* page_table_addr = PMM::getInstance().allocate_frame();

        Page& page_directory = *(new (page_directory_addr) Page{});
        Page& page_table = *(new (page_table_addr) Page{});

        page_directory[0] = PageEntry(page_table.address(), 0x3);
        for (size_t i = 1; i < 1024; i++) {
            page_directory[i] = PageEntry(0, 0);
        }

        page_table[0] = PageEntry(0x100000, 0x7);
        for (size_t i = 1; i < 1024; i++) {
            page_table[i] = PageEntry(0, 0);
        }

        return page_directory_addr;
    }

};
