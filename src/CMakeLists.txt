set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_LINKER "ld.lld")
set(CMAKE_CXX_LINK_FLAGS "../lkd.ld -nostdlib")
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_LINKER} <CMAKE_CXX_LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

add_executable(lkd "")

set_target_properties(lkd PROPERTIES LINK_DEPENDS ../lkd.ld)

target_sources(lkd PUBLIC
    includes/io.hpp
    serial/serial.cpp
    boot.S
    main.cpp

    utils/bitmap.cpp

    gdt/gdt_load.S
    gdt/gdt.cpp

    pmm/pmm.cpp
)

target_include_directories(lkd PRIVATE "includes")

target_compile_options(lkd PRIVATE
    $<$<COMPILE_LANGUAGE:CXX,ASM>:--target=i686-pc-none-elf;-march=i686;-m32;-nostdlibinc;-fno-stack-protector;-fno-threadsafe-statics>
    $<$<COMPILE_LANGUAGE:CXX>:-ffreestanding;-fno-exceptions;-fno-rtti;-fno-builtin;-O0>
)
