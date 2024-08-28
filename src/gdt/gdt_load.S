.extern gdtr

.section .text
.global gdt_load
gdt_load:
    lgdt gdtr
    ljmp $0x08, $.reload_segments

.reload_segments:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret
