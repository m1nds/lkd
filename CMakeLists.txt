cmake_minimum_required(VERSION 3.29)

project(lkd LANGUAGES CXX ASM)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
