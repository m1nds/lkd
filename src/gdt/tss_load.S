.section .text
.global tss_load
tss_load:
    movw $0x28, %ax
    ltr %ax
    ret
