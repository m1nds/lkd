set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_LINKER "ld.lld")

set(CMAKE_CXX_LINK_FLAGS "-nostdlib -T ${CMAKE_SOURCE_DIR}/lkd.ld -no-pie -melf_i386")
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_LINKER} <CMAKE_CXX_LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

add_executable(lkd "")

set_target_properties(lkd PROPERTIES LINK_DEPENDS ../lkd.ld)

target_sources(lkd PUBLIC
    includes/io.hpp
    outputs/serial.cpp
    outputs/vga.cpp
    boot.S
    main.cpp

    utils/bitmap.cpp
    utils/string.cpp
    utils/tar.cpp

    gdt/gdt_load.S
    gdt/tss_load.S
    gdt/gdt.cpp

    vmm/paging.cpp
    vmm/kmalloc.cpp
    vmm/kmalloc_utils.cpp
    vmm/bucket.cpp

    interrupts/handlers.cpp
    interrupts/isr.S
    interrupts/irq.S
    interrupts/idt.cpp
    interrupts/pic.cpp

    devices/timer.cpp
    devices/keyboard.cpp

    userland/syscalls.cpp

    pmm/pmm.cpp
)

target_include_directories(lkd PRIVATE
    includes
    outputs/includes
    gdt/includes
    vmm/includes
    interrupts/includes
    pmm/includes
    utils/includes
    devices/includes
    userland/includes
)

target_compile_options(lkd PRIVATE
    $<$<COMPILE_LANGUAGE:CXX,ASM>:--target=i686-pc-none-elf;-march=i686;-m32;-nostdlibinc;-fno-stack-protector;-fno-threadsafe-statics;-ggdb3>
    $<$<COMPILE_LANGUAGE:CXX>:-ffreestanding;-fno-exceptions;-fno-rtti;-fno-builtin;-O0;-fno-pie;-fno-pic>
)

