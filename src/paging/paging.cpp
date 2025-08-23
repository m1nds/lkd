#include <new>

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <paging.hpp>
#include <utils.hpp>
#include <serial.hpp>
#include <pmm.hpp>

using namespace pmm;

namespace vmm {
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

    uint32_t* Page::address() {
        return reinterpret_cast<uint32_t*>(this);
    }

    Page& Page::get_current_pd() {
        uint32_t* pd_addr;
        __asm__ volatile("mov %%cr3, %0" : "=r"(pd_addr));

        pd_addr = reinterpret_cast<uint32_t*>(P2V(pd_addr));
        return *(reinterpret_cast<Page*>(pd_addr));
    }

    void Page::map_page(uint32_t phys_addr, uint32_t virt_addr, uint32_t flags) {
        uint32_t* pd = this->address();

        pd = reinterpret_cast<uint32_t*>(P2V(pd));

        uint32_t pd_idx = (virt_addr >> 22) & 0x3FF;
        uint32_t pt_idx = (virt_addr >> 12) & 0x3FF;

        uint32_t *pt = reinterpret_cast<uint32_t*>(P2V(pd[pd_idx] & ~(0xFFF)));

        pt[pt_idx] = (phys_addr & ~(0xFFF)) | (flags & 0xFFF);

        __asm__ volatile("invlpg (%0)" ::"r"(virt_addr) : "memory");
    }
};
