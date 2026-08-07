set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_LINKER "ld.lld")

add_executable(lkd "")

set(CMAKE_CXX_LINK_FLAGS "-target ${ARCH}-unknown-windows -nostdlib -Wl,-entry:efi_main -Wl,-subsystem:efi_application -fuse-ld=lld-link")

set(GNUEFI_INC ${CMAKE_SOURCE_DIR}/gnu-efi/inc)

target_compile_options(lkd PRIVATE
	$<$<COMPILE_LANGUAGE:CXX,ASM>:-target ${ARCH}-unknown-windows;-fshort-wchar;-mno-red-zone>
	$<$<COMPILE_LANGUAGE:CXX>:-ffreestanding;-fno-rtti;-fno-exceptions;-I${GNUEFI_INC};-I${GNUEFI_INC}/${ARCH};-I${GNUEFI_INC}/protocol>
)

target_sources(lkd PUBLIC main.cpp)

add_subdirectory(arch/${ARCH})
add_subdirectory(uart)
add_subdirectory(memory)
