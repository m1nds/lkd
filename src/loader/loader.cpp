#include <loader.hpp>
#include <serial.hpp>
#include <pmm.hpp>
#include <paging.hpp>
#include <stdint.h>
#include <string.h>

extern "C" void enter_userland(void* addr);

Elf32::Elf32(void* elf) {
    this->elf_header = reinterpret_cast<Elf32_Ehdr*>(elf);
    this->entry_point = nullptr;
}

bool Elf32::verify_header() {
    uint8_t elf_magic[] = { '\x7f', 'E', 'L', 'F' };
    if (memcmp(this->elf_header->e_ident, elf_magic, 4)) {
        return false;
    }

    return true;
}

void Elf32::load(vmm::Page& pd) {
    serial::Serial s{};

    if (!this->verify_header()) {
        s.write_str("[ELF] Invalid ELF magic!\n");
        return;
    }

    uint8_t* elf_ptr = reinterpret_cast<uint8_t*>(this->elf_header);
    Elf32_Ehdr* elf = reinterpret_cast<Elf32_Ehdr*>(this->elf_header);

    uint32_t ph_offset = this->elf_header->e_phoff;
    uint32_t ph_count = this->elf_header->e_phnum;
    uint32_t ph_size = this->elf_header->e_phentsize;

    Elf32_Phdr *current_phdr;
    for (uint32_t i = 0; i < ph_count; i++) {
        current_phdr = reinterpret_cast<Elf32_Phdr*>(elf_ptr + ph_offset + i * ph_size);
        s.kprintf("=== Program Header ===\n");
        s.kprintf("    Type: %x\n", current_phdr->p_type);
        s.kprintf("    Vaddr: %x\n", current_phdr->p_vaddr);
        s.kprintf("    Paddr: %x\n", current_phdr->p_paddr);

        if (current_phdr->p_type == PT_LOAD && current_phdr->p_filesz != 0) {
            uint32_t phys_addr = reinterpret_cast<uint32_t>(pmm::PMM::getInstance().allocate_frame());
            uint8_t* virt_addr = reinterpret_cast<uint8_t*>(current_phdr->p_vaddr);

            pd.map_page(phys_addr, current_phdr->p_vaddr, 0x7);

            void* curr_data = reinterpret_cast<void*>(elf_ptr + current_phdr->p_offset);

            memcpy(virt_addr, curr_data, current_phdr->p_filesz);
            //memset(virt_addr + current_phdr->p_filesz, 0, current_phdr->p_memsz - current_phdr->p_filesz);
        }
    }

    this->entry_point = reinterpret_cast<void*>(elf->e_entry);
}

void Elf32::run_process() {
    enter_userland(this->entry_point);
}
