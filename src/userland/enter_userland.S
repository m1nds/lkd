.global enter_userland
enter_userland:
    mov $0x23, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    mov %esp, %eax
    push $0x23
    push %eax
    pushf
    push $0x1b
    push $0x4000
    iret

.global userland_entry
userland_entry:
    mov $0x8000, %ebp
    mov $0x8000, %esp
    xor %eax, %eax
    int $0x80
loop:
    jmp loop

