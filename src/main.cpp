#include <io.hpp>
#include <serial.hpp>
#include <gdt.hpp>
#include <multiboot.h>

extern "C" void kmain(multiboot_info_t* mbd) {
    serial::Serial s{};

    gdt_init();

    s.write_str("[MAIN] GDT is OK\n");

    s.kprintf("[KPRINTF] Testing kprintf : in your %x %d\n", 0xF4CE, 4269);
}
