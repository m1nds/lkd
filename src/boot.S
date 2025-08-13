.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
.global stack_bottom
stack_bottom:
.skip 16384 # 16 KiB
.global stack_top
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    movl $stack_top, %esp

    push %eax
    push %ebx
    call kmain

    cli
1:
    hlt
    jmp 1b

.size _start, . - _start

