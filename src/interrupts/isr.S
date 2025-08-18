.extern isr_handler

.macro ISR_ERR num
.global isr_\num
isr_\num:
    push $\num
    jmp isr_common_stub
.endm

.macro ISR_NO_ERR num
.global isr_\num
isr_\num:
    push $0
    push $\num
    jmp isr_common_stub
.endm

isr_common_stub:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi

    call isr_handler

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

# Defining ISRs
ISR_NO_ERR 0
ISR_NO_ERR 1
ISR_NO_ERR 2
ISR_NO_ERR 3
ISR_NO_ERR 4
ISR_NO_ERR 5
ISR_NO_ERR 6
ISR_NO_ERR 7
ISR_ERR 8
ISR_NO_ERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NO_ERR 15
ISR_NO_ERR 16
ISR_ERR 17
ISR_NO_ERR 18
ISR_NO_ERR 19
ISR_NO_ERR 20
ISR_NO_ERR 21
ISR_NO_ERR 22
ISR_NO_ERR 23
ISR_NO_ERR 24
ISR_NO_ERR 25
ISR_NO_ERR 26
ISR_NO_ERR 27
ISR_NO_ERR 28
ISR_NO_ERR 29
ISR_ERR 30
ISR_NO_ERR 31

.global isr_table
isr_table:
    .long isr_0
    .long isr_1
    .long isr_2
    .long isr_3
    .long isr_4
    .long isr_5
    .long isr_6
    .long isr_7
    .long isr_8
    .long isr_9
    .long isr_10
    .long isr_11
    .long isr_12
    .long isr_13
    .long isr_14
    .long isr_15
    .long isr_16
    .long isr_17
    .long isr_18
    .long isr_19
    .long isr_20
    .long isr_21
    .long isr_22
    .long isr_23
    .long isr_24
    .long isr_25
    .long isr_26
    .long isr_27
    .long isr_28
    .long isr_29
    .long isr_30
    .long isr_31
