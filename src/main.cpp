#include <io.hpp>
#include <serial.hpp>
#include <gdt.hpp>
#include <pmm.hpp>

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
}
