cmake_minimum_required(VERSION 3.20)

project(lkd LANGUAGES CXX ASM)

set(ARCH "x86_64" CACHE STRING "Target architecture (x86_64 or aarch64)")
set_property(CACHE ARCH PROPERTY STRINGS "x86_64" "aarch64")

message(STATUS "Configuring kernel build for architecture: ${ARCH}")

add_subdirectory(src)
