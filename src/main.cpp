#include <io.hpp>
#include <serial.hpp>
#include <gdt.hpp>
#include <pic.hpp>
#include <pmm.hpp>
#include <idt.hpp>
#include <paging.hpp>
#include <timer.hpp>

#include <multiboot.h>

extern "C" void kmain(multiboot_info_t* mbd, uint32_t magic) {
    serial::Serial s{};

    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        s.kprintf("[MAIN] Invalid Multiboot Magic, got : %x, expected %x!\n", magic, MULTIBOOT_BOOTLOADER_MAGIC);
        return;
    }

    gdt::gdt_init();
    s.write_str("[MAIN] GDT > OK\n");

    idt::idt_init();
    s.write_str("[MAIN] IDT > OK\n");

    pmm::PMM::getInstance().init(mbd);
    s.write_str("[MAIN] PMM > OK\n");

    timer::PIT timer_pit{1000};
    s.write_str("[MAIN] Timer (PIT) > OK\n");

    //vmm::kernel_init_paging();
    //s.write_str("[MAIN] Kernel Paging > OK\n");
}
