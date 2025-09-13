#!/bin/sh
set -e

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_ASM_COMPILER=clang++
cmake --build build

mkdir -p isodir/boot/grub

cp grub.cfg isodir/boot/grub
cp build/src/lkd isodir/boot

cd initrd
tar -cf lkd.tar $(ls)
mv lkd.tar ../isodir/boot
cd ..

grub-mkrescue -o lkd.iso isodir
