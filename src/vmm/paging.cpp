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

    uint32_t PageEntry::value() {
        return this->_value;
    }

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
        Page& pd = *(reinterpret_cast<Page*>(P2V(this->address())));

        uint32_t pd_idx = (virt_addr >> 22) & 0x3FF;
        uint32_t pt_idx = (virt_addr >> 12) & 0x3FF;

        PageEntry& pd_entry = pd[pd_idx];
        if (! (pd_entry.value() & 0x1)) {
            Page& new_page = *(reinterpret_cast<Page*>(P2V(pmm::PMM::getInstance().allocate_frame())));
            memset(&new_page, 0, 0x1000);

            pd[pd_idx] = PageEntry(reinterpret_cast<uint32_t>(V2P(&new_page)), 0x7);
        }

        Page& pt = *(reinterpret_cast<Page*>(P2V(pd_entry.value() & ~(0xFFF))));
        pt[pt_idx] = PageEntry(phys_addr, flags);

        __asm__ volatile("invlpg (%0)" ::"r"(virt_addr) : "memory");
    }
};
