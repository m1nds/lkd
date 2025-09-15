#include <loader.hpp>
#include <serial.hpp>
#include <stdint.h>
#include <string.h>

Elf32::Elf32(void* elf) {
    this->elf_header = reinterpret_cast<Elf32_Ehdr*>(elf);
}

void Elf32::load() {
    serial::Serial s{};

    uint8_t elf_magic[] = { '\x7f', 'E', 'L', 'F' };
    if (memcmp(this->elf_header->e_ident, elf_magic, 4)) {
        s.write_str("[ELF] Invalid ELF magic!\n");
        return;
    }

    uint8_t* elf_ptr = reinterpret_cast<uint8_t*>(this->elf_header);

    uint32_t ph_offset = this->elf_header->e_phoff;
    uint32_t ph_count = this->elf_header->e_phnum;
    uint32_t ph_size = this->elf_header->e_phentsize;

    Elf32_Phdr *current_phdr;
    for (uint32_t i = 0; i < ph_count; i++) {
        current_phdr = reinterpret_cast<Elf32_Phdr*>(elf_ptr + ph_offset + i * ph_size);

        s.kprintf("=== Program Entry ===\n");
        s.kprintf("    Type: %x\n", current_phdr->p_type);
    }

    uint32_t sh_offset = this->elf_header->e_shoff;
    uint32_t sh_count = this->elf_header->e_shnum;
    uint32_t sh_size = this->elf_header->e_shentsize;
    uint32_t sh_strtable_off = this->elf_header->e_shstrndx;

    Elf32_Shdr *current_shdr;
    Elf32_Shdr *sh_strtable = reinterpret_cast<Elf32_Shdr*>(elf_ptr + sh_offset + sh_strtable_off * sh_size);

    for (uint32_t i = 0; i < sh_count; i++) {
        if (i == sh_strtable_off) {
            continue;
        }

        current_shdr = reinterpret_cast<Elf32_Shdr*>(elf_ptr + sh_offset + i * sh_size);
        s.kprintf("=== Section Entry ===\n");
        s.kprintf("    Name: %s\n", elf_ptr + sh_strtable->sh_offset + current_shdr->sh_name);
        s.kprintf("    Type: %x\n", current_shdr->sh_type);
    }

}
