#include <io.hpp>
#include <serial.hpp>
#include <gdt.hpp>
#include <pmm.hpp>
#include <idt.hpp>
#include <paging.hpp>

#include <multiboot.h>

void task1(void) {
       serial::Serial s{};
       char *msg = (char*) 0x40000100; /* le message sera stocké en 0x100100 */
       msg[0] = 't';
       msg[1] = 'a';
       msg[2] = 's';
       msg[3] = 'k';
       msg[4] = '1';
       msg[5] = '\n';
       msg[6] = 0;

       s.write_str(msg);
       while(1);
       return; /* never go there */
}

extern "C" void kmain(multiboot_info_t* mbd, uint32_t magic) {
    serial::Serial s{};

    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        s.kprintf("[MAIN] Invalid Multiboot Magic, got : %x, expected %x!\n", magic, MULTIBOOT_BOOTLOADER_MAGIC);
        return;
    }

    gdt::gdt_init();
    s.write_str("[MAIN] GDT > OK\n");

    pmm::PMM::getInstance().init(mbd);
    s.write_str("[MAIN] PMM > OK\n");

    //vmm::kernel_init_paging();
    //s.write_str("[MAIN] Kernel Paging > OK\n");

    idt::idt_init();
    s.write_str("[MAIN] IDT > OK\n");

    s.write_str("[MAIN] Launching 'task1'...\n");
    void* pd_ptr = vmm::get_task_pd((void*) &task1);

    asm volatile (
    "cli \n"
    // Load task's page directory
    "mov %0, %%cr3 \n"
    
    // Set up user mode segments
    "mov $0x23, %%ax \n"
    "mov %%ax, %%ds \n"
    "mov %%ax, %%es \n"
    "mov %%ax, %%fs \n"
    "mov %%ax, %%gs \n"
    
    // Create IRET stack frame
    "pushl $0x23 \n"       // User SS
    "pushl $0x40000F00 \n" // User ESP
    "pushfl \n"            // Current EFLAGS
    "popl %%eax \n"
    "orl $0x200, %%eax \n" // Enable interrupts
    "pushl %%eax \n"       // New EFLAGS
    "pushl $0x1B \n"       // User CS
    "pushl %1 \n"          // EIP (task1)
    "iret \n"
    : 
    : "r"(pd_ptr), "r"((uint32_t)&task1)
    : "eax", "memory"
);
}
