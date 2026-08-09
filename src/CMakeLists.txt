add_executable(lkd "")

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/lkd.ld)
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_LINKER} -T ${LINKER_SCRIPT} <CMAKE_CXX_LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

target_link_options(lkd PRIVATE
    -target ${ARCH}-unknown-none-elf
    -nostdlib
    -fuse-ld=lld
    -T ${CMAKE_CURRENT_SOURCE_DIR}/src/arch/${ARCH}/lkd.ld
)

target_compile_options(lkd PRIVATE
        $<$<COMPILE_LANGUAGE:CXX,ASM>:-target ${ARCH}-unknown-none-elf;-fshort-wchar;-mno-red-zone>
        $<$<COMPILE_LANGUAGE:CXX>:-ffreestanding;-fno-rtti;-fno-exceptions;-fno-stack-protector>
)

target_sources(lkd PUBLIC main.cpp)

add_subdirectory(arch/${ARCH})
add_subdirectory(uart)
