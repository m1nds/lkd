#include <io.hpp>
#include <vga.hpp>
#include <utils.hpp>
#include <tar.hpp>
#include <serial.hpp>
#include <gdt.hpp>
#include <string.h>
#include <pic.hpp>
#include <pmm.hpp>
#include <idt.hpp>
#include <paging.hpp>
#include <timer.hpp>
#include <keyboard.hpp>
#include <loader.hpp>

#include <multiboot.h>
#include <kmalloc.hpp>

void load_initrd(multiboot_info_t* mbd) {
    serial::Serial s{};

    multiboot_module_t* mods = reinterpret_cast<multiboot_module_t*>(P2V(mbd->mods_addr));
    uint32_t start = P2V(mods->mod_start);

    vmm::Page& pd = vmm::Page::get_current_pd();
    pd.map_page(V2P(start), start, 0x3);

    utils::Tar* tar = utils::parse_tar(start);
    while (tar != nullptr) {
        s.kprintf("File: %s\n", tar->header.filename);
        if (strcmp(tar->header.filename, "test_exec") == 0) {
            Elf32 elf = Elf32(tar->data);
            elf.load();
        } else {
            s.kprintf("Content: %s\n", tar->data);
        }

        tar = tar->next;
    }
}

extern "C" void kmain(multiboot_info_t* mbd, uint32_t magic) {
    serial::Serial s{};
    vga::VGA v{};

    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        s.kprintf("[MAIN] Invalid Multiboot Magic, got : %x, expected %x!\n", magic, MULTIBOOT_BOOTLOADER_MAGIC);
        return;
    }

    gdt::gdt_init();
    s.write_str("[MAIN] GDT > OK\n");
    v.write_str("[MAIN] GDT > OK\n");

    idt::idt_init();
    s.write_str("[MAIN] IDT > OK\n");
    v.write_str("[MAIN] IDT > OK\n");

    pmm::PMM::getInstance().init(mbd);
    s.write_str("[MAIN] PMM > OK\n");
    v.write_str("[MAIN] PMM > OK\n");

    timer::PIT timer_pit{1000};
    s.write_str("[MAIN] Timer (PIT) > OK\n");
    v.write_str("[MAIN] Timer (PIT) > OK\n");

    keyboard::Keyboard keyboard{};
    s.write_str("[MAIN] Keyboard > OK\n");
    v.write_str("[MAIN] Keyboard > OK\n");

    load_initrd(mbd);

    while (true) {
        s.kprintf("[KEYBOARD] Write '%c'\n", keyboard::Keyboard::getchar());
    }
}
