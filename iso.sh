#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/fortuna.kernel isodir/boot/fortuna.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "fortuna" {
	multiboot /boot/fortuna.kernel
}
EOF
grub-mkrescue -o fortuna.iso isodir
