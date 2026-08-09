cmake_minimum_required(VERSION 3.20)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_LINKER "ld.lld")

set(ARCH "x86_64" CACHE STRING "Target architecture (x86_64 or aarch64)")
set_property(CACHE ARCH PROPERTY STRINGS "x86_64" "aarch64")

project(lkd LANGUAGES CXX ASM)

message(STATUS "Configuring kernel build for architecture: ${ARCH}")

add_subdirectory(src)
