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
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>:-ggdb3>
        $<$<COMPILE_LANGUAGE:CXX,ASM>:-target ${ARCH}-unknown-none-elf;-fshort-wchar;-mno-red-zone;-O0>
        $<$<COMPILE_LANGUAGE:CXX>:-ffreestanding;-fno-rtti;-fno-exceptions;-fno-stack-protector;-fno-threadsafe-statics>
)

target_sources(lkd PUBLIC main.cpp)

add_subdirectory(arch/${ARCH})
add_subdirectory(uart)
add_subdirectory(memory)
add_subdirectory(utils)
