#!/bin/sh
set -e

ARCH="${1:-x86_64}"

case "$ARCH" in
        x86_64)  EFI_NAME=BOOTX64.EFI ;;
        aarch64) EFI_NAME=BOOTAA64.EFI ;;
        *) echo "usage: $0 [x86_64|aarch64]" >&2; exit 1 ;;
esac

# Build Limine bootloader
cd limine
./bootstrap
./configure --enable-bios --enable-bios-cd --enable-uefi-${ARCH} --enable-uefi-cd
make
cd ..

# Build the kernel
cmake -S . -B build -DARCH="$ARCH"
cmake --build build

# Copy kernel to ISO
mkdir -p iso/boot
cp -v build/src/lkd iso/boot/

# Copy limine config and utils to ISO
mkdir -p iso/boot/limine
cp -v limine.conf limine/bin/limine-bios.sys limine/bin/limine-bios-cd.bin limine/bin/limine-uefi-cd.bin iso/boot/limine

# Copy Limine application to ISO
mkdir -p iso/EFI/BOOT
cp -v limine/bin/${EFI_NAME} iso/EFI/BOOT/

# Create the bootable ISO.
xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        iso -o lkd.iso

# Install Limine stage 1 and 2 for legacy BIOS boot.
./limine/bin/limine bios-install lkd.iso
