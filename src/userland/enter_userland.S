//void enter_userland(void* addr);
.global enter_userland
enter_userland:
    mov 4(%esp), %ebx
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
    push %ebx
    iret
