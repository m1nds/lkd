# Learning Kernel Development

## Project Description

This is a personnal project I made to understand how to implement a `x86` kernel.
I want to achieve a functionning kernel that will be able to run processes in userland.
So far, I have implemented these features.

- Global Descriptor Table for Segmentation
- `PIC` and Interrupt Descriptor Table for `Software` and `Hardware` interruptions
- Visual and output utilities using VGA Text mode and Serial I/O
- Hardware devices such as a `PIT` timer and a `PS/2` Keyboard
- `Kernel Pagination` and mapping utilities
- `Dynamic memory allocation` with a `bucket-style` implementation
- `Initrd` support using a `Tar` parser
- Simple `ELF` parser

I still need to achieve some features to have a fully functionnal kernel. Here are some of the features I plan to implement.
- Proper program loading in memory using ELF Loader and Memory utilities
- Proper userland switching
- Preemptive scheduling using the PIT timer

## How to build ?

I personnaly use `NixOS`. You can install dependencies using the following command.
```sh
nix-shell
```

The build toolchain is composed of `CMake`, `Ninja`, `Clang` and `LLVM`. You should install these before trying to build. I then provided a `create-iso.sh` script that create all the necessary things (CMake build, ISO, initrd, etc...)

## Running the kernel ?

I personnaly used `QEMU` to test and debug the kernel. You should be able to run the kernel using the follow command.
```sh
qemu-system-i386 -cdrom lkd.iso -serial stdio
```

## Running tests

I made a `run-test.sh` script to run the tests. I use `Google Test` as a testing library. It is still hard to implement unit tests for a kernel so there are not enough tests.
