#!/bin/sh

if [ ! -d build ]; then
    echo "Need to build first !"
    exit 1
fi

if [ ! -f build/OVMF_VARS.fd ] || [ ! -f build/OVMF_CODE.fd ]; then
    cp -r "${OVMF_DIR}/OVMF_CODE.fd" ./build
    cp -r "${OVMF_DIR}/OVMF_VARS.fd" ./build
fi

sudo qemu-system-x86_64 \
  -drive if=pflash,format=raw,unit=0,file="./build/OVMF_CODE.fd",readonly=on \
  -drive if=pflash,format=raw,unit=1,file="./build/OVMF_VARS.fd" \
  -serial stdio \
  -cdrom lkd.iso
