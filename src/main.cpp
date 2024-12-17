#include <io.hpp>
#include <serial.hpp>
#include <gdt.hpp>
#include <pmm.hpp>
#include <idt.hpp>

#include <multiboot.h>

extern "C" void kmain(multiboot_info_t* mbd, uint32_t magic) {
    serial::Serial s{};

    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        s.kprintf("[MAIN] Invalid Multiboot Magic, got : %x, expected %x!\n", magic, MULTIBOOT_BOOTLOADER_MAGIC);
        return;
    }

    gdt_init();
    s.write_str("[MAIN] GDT > OK\n");

    pmm::PMM::getInstance().init(mbd);
    s.write_str("[MAIN] PMM > OK\n");

    idt::idt_init();
    s.write_str("[MAIN] IDT > OK\n");

    __asm__ volatile ("int $0x3");
}
