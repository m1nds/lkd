ENTRY(_start)
SECTIONS
{
    . = 1M;
    kernelStart = .;
    .text :
    {
            *(.multiboot)
            *(.text)
    }

    .rodata :
    {
            *(.rodata)
    }

    .data :
    {
            *(.data)
    }

    .bss :
    {
            *(COMMON)
            *(.bss)
    }
    kernelEnd = .;
}

