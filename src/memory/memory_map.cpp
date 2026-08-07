#include <efi.h>
#include <efilib.h>

#include <memory_map.hpp>
#include <uart.hpp>

EFI_STATUS iterate_memory_map(EFI_SYSTEM_TABLE* st) {
    EFI_STATUS Status = 1;
    UINTN MemoryMapSize = 0;
    EFI_MEMORY_DESCRIPTOR* MemoryMap = NULL;
    UINTN MapKey = 0;
    UINTN DescriptorSize = 0;
    UINT32 DescriptorVersion = 0;

    Status = st->BootServices->GetMemoryMap(
        &MemoryMapSize, NULL, &MapKey, &DescriptorSize, &DescriptorVersion);
    if (Status != EFI_BUFFER_TOO_SMALL) {
        UARTG::kprintf("%s\n", "Failed to retrieve memory map size!");
        return Status;
    }

    // New allocation can split a free memory into two map elements, meaning we
    // have to add 2.
    MemoryMapSize += 2 * DescriptorSize;

    Status = st->BootServices->AllocatePool(EfiLoaderData, MemoryMapSize,
                                            (VOID**)&MemoryMap);
    if (Status != EFI_SUCCESS) {
        UARTG::kprintf("%s\n", "Failed to allocate memory map!");
        return Status;
    }

    Status =
        st->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey,
                                       &DescriptorSize, &DescriptorVersion);
    if (Status != EFI_SUCCESS) {
        UARTG::kprintf("%s\n", "Failed to retrieve memory map!");
        st->BootServices->FreePool(MemoryMap);
        return Status;
    }

    EFI_MEMORY_DESCRIPTOR* end =
        NEXT_MEMORY_DESCRIPTOR(MemoryMap, MemoryMapSize);
    EFI_MEMORY_DESCRIPTOR* curr;
    for (curr = MemoryMap; curr < end;
         curr = NEXT_MEMORY_DESCRIPTOR(curr, DescriptorSize)) {
        UARTG::kprintf("=== Memory Map Descriptor ===\n");
        UARTG::kprintf("Type = %x\n", curr->Type);
        UARTG::kprintf("PhysicalStart = %X\n", curr->PhysicalStart);
        UARTG::kprintf("VirtualStart = %X\n", curr->VirtualStart);
        UARTG::kprintf("NumberOfPages = %X\n", curr->NumberOfPages);
        UARTG::kprintf("Attribute = %X\n", curr->Attribute);
    }

    st->BootServices->FreePool(MemoryMap);
    return Status;
}
