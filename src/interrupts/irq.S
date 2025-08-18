.extern irq_handler

.macro IRQ num
.global irq_\num
irq_\num:
    push $\num
    push $\num
    jmp irq_common_stub
.endm

irq_common_stub:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi

    call irq_handler

    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    add $8, %esp
    iret

# Defining IRQs
IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15

.global irq_table
irq_table:
    .long irq_0
    .long irq_1
    .long irq_2
    .long irq_3
    .long irq_4
    .long irq_5
    .long irq_6
    .long irq_7
    .long irq_8
    .long irq_9
    .long irq_10
    .long irq_11
    .long irq_12
    .long irq_13
    .long irq_14
    .long irq_15
