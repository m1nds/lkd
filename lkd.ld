ENTRY (_start)
SECTIONS{
    . = 0x00100000;
    kernel_start = .;

    .text :{
        text_start = .;
        *(.multiboot)
        *(.text)
    }

    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }

    .data ALIGN (0x1000) : {
        *(.data)
        end_data = .;
    }

    .bss : {
        sbss = .;
        *(COMMON)
        *(.bss)
        ebss = .;
        bitmap_start = .;
        /* . = . + 0x10000; 64 KiB reserved for page frame bitmap */
        bitmap_end = .;
    }

    kernel_end = .;
}
