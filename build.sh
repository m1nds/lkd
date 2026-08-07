#!/bin/sh
set -e

ARCH="${1:-x86_64}"

case "$ARCH" in
	x86_64)  EFI_NAME=BOOTX64.EFI ;;
	aarch64) EFI_NAME=BOOTAA64.EFI ;;
	*) echo "usage: $0 [x86_64|aarch64]" >&2; exit 1 ;;
esac

cmake -S . -B build -DARCH="$ARCH"
cmake --build build

dd if=/dev/zero of=fat.img bs=1k count=1440
mformat -i fat.img -f 1440 ::
mmd -i fat.img ::/EFI
mmd -i fat.img ::/EFI/BOOT
mcopy -i fat.img build/src/lkd "::/EFI/BOOT/$EFI_NAME"

mkdir -p iso
cp fat.img iso
xorriso -as mkisofs -R -f -e fat.img -no-emul-boot -o lkd.iso iso
