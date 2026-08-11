#!/bin/bash

# biar gampang
OSBOOT_DIR="osboot"
ISO_DIR="iso_temp"
OUTPUT_ISO="$OSBOOT_DIR/farewell.iso"

if [ ! -f "$OSBOOT_DIR/bzImage" ] || [ ! -f "$OSBOOT_DIR/single.gz" ] || [ ! -f "$OSBOOT_DIR/multi.gz" ]; then
	echo "[!] ERROR: bzImage, single.gz, atau multi.gz not found in $OSBOOT_DIR."
	echo "Did you run kernel.sh, single.sh, and multi.sh first?"
	exit 1
fi

echo "Scaffolding ISO directory structure..."
rm -rf $ISO_DIR
mkdir -p $ISO_DIR/boot/grub

echo "Copying kernel and initramfs to ISO directory..."
cp $OSBOOT_DIR/bzImage $ISO_DIR/boot/
cp $OSBOOT_DIR/single.gz $ISO_DIR/boot/
cp $OSBOOT_DIR/multi.gz $ISO_DIR/boot/

echo "Configuring GRUB bootloader..."

cat <<'EOF' >$ISO_DIR/boot/grub/grub.cfg
set timeout=15
set default=0

menuentry "FarewellOS - Single User Mode" {
    linux /boot/bzImage console=tty0
    initrd /boot/single.gz
}

menuentry "FarewellOS - Multi User Mode" {
    linux /boot/bzImage console=tty0
    initrd /boot/multi.gz
}
EOF

echo "Building bootable ISO image with grub-mkrescue..."

grub-mkrescue -o $OUTPUT_ISO $ISO_DIR 2>/dev/null
if [ $? -gt 0 ]; then
	echo "[!] ERROR: Failed to create ISO image."
	exit 1
fi

echo "Cleaning up..."
rm -rf $ISO_DIR

echo "Done! ISO image created at $OUTPUT_ISO"
