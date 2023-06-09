#include <stdint.h>

#include "tty.h"
#include "interrupts.h"

__attribute__ ((noreturn)) void isr_handler(uint32_t int_number) {
    char test[] = "INT (Software Interrupt)\n";
    print(test);

    __asm__ volatile ("cli; hlt");
}
