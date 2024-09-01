#!/bin/sh
set -e

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_ASM_COMPILER=clang++
cmake --build build

cd build && ctest

