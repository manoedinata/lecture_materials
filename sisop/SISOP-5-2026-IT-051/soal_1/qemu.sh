#!/bin/bash

# biar gampang
KERNEL="osboot/bzImage"
FS_SINGLE="osboot/single.gz"
FS_MULTI="osboot/multi.gz"
ISO="osboot/farewell.iso"

if ! command -v qemu-system-x86_64 &>/dev/null; then
	echo "[!] ERROR: qemu-system-x86_64 not found."
	exit 1
fi

# Intinya, kalau all, maka GRUB akan muncul karena kita me-load file ISO utuh, jadi bisa
# milih mau boot single / multi.
# Kalau --single atau --multi, maka kita langsung me-load bzImage dan initramfs yang sesuai,
# jadi tidak pakai GRUB dan langsung boot ke mode yang dipilih.

case "$1" in
--single)
	echo "FarewellOS - Single User Mode"
	qemu-system-x86_64 -kernel $KERNEL -initrd $FS_SINGLE -m 512M -append "console=ttyS0" -nographic
	;;

--multi)
	echo "FarewellOS - Multi User Mode"
	qemu-system-x86_64 -kernel $KERNEL -initrd $FS_MULTI -m 512M -append "console=ttyS0" -nographic
	;;

--all)
	echo "FarewellOS - Booting from ISO"
	qemu-system-x86_64 -cdrom $ISO -m 512M
	;;

*)
	echo "================================================="
	echo "   QEMU Boot Script - Custom Linux OS            "
	echo "================================================="
	echo "Cara Penggunaan: ./qemu.sh [PARAMETER]"
	echo ""
	echo "Parameter:"
	echo "  --single   : Boot single-user filesystem langsung"
	echo "  --multi    : Boot multi-user filesystem langsung"
	echo "  --all      : Boot dari ISO (Pilih via menu GRUB)"
	echo "================================================="
	exit 1
	;;
esac
