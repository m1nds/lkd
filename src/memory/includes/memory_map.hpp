#pragma once

#include <efi.h>
#include <efilib.h>

#define NEXT_MEMORY_DESCRIPTOR(Descriptor, DescriptorSize)                     \
    (EFI_MEMORY_DESCRIPTOR*)((char*)(Descriptor) + (DescriptorSize))

EFI_STATUS iterate_memory_map(EFI_SYSTEM_TABLE* st);
