#include <efi.h>
#include <efilib.h>

#include <memory_map.hpp>
#include <uart.hpp>

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    EFI_STATUS Status;
    EFI_SYSTEM_TABLE* ST = SystemTable;

    UARTG::kprintf("Booting LKD...\n");
    Status = iterate_memory_map(ST);

    for (;;)
        ;

    return Status;
}
