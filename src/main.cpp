#include <uart.hpp>
#include <pmm.hpp>

#include <limine.h>

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

void hlt() {
    for (;;)
        __asm__ volatile ("hlt");
        ;
}

extern "C" void kmain() {

    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        UARTG::kprintf("Limine Base Revision not supported!\n");
        hlt();
    }

    UARTG::kprintf("Booting LKD...\n");
    PMM::getInstance().init();
    hlt();
}
