#!/bin/sh
set -e

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_ASM_COMPILER=clang++
cmake --build build

mkdir -p isodir/boot/grub

cp grub.cfg isodir/boot/grub
cp build/lkd isodir/boot

grub-mkrescue -o lkd.iso isodir
