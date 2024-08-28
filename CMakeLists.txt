cmake_minimum_required(VERSION 3.29)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(lkd LANGUAGES CXX ASM)

add_executable(lkd "")
target_sources(lkd PUBLIC
    src/includes/io.hpp
    src/serial/serial.cpp
    src/boot.S
    src/main.cpp

    src/gdt/gdt_load.S
    src/gdt/gdt.cpp
)

target_include_directories(lkd PRIVATE "src/includes")

target_compile_options(lkd PRIVATE
    $<$<COMPILE_LANGUAGE:CXX,ASM>:--target=i686-pc-none-elf;-march=i686;-m32;-nostdlibinc;-fno-stack-protector>
    $<$<COMPILE_LANGUAGE:CXX>:-ffreestanding;-fno-exceptions;-fno-rtti;-fno-builtin;-O0>
)

target_link_options(lkd PRIVATE --target=i686-pc-none-elf -m32 -nostdlib -T lkd.ld -WL,--no-pie)

set(CMAKE_LINKER "ld.lld")
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_LINKER} <CMAKE_CXX_LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
