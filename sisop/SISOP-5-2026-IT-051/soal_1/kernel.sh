#!/usr/bin/env bash

set -eo pipefail # bail-out kalo error

if [ -d "linux" ]; then
	echo "Linux kernel source code already exists. Skipping cloning."
else
	echo "Cloning Linux kernel source code..."
	git clone https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git \
		-b v6.1.1 \
		--depth 1
fi
cd linux

echo "Building Linux kernel..."
make defconfig O=temp_out/
make menuconfig O=temp_out/
make -j$(nproc) O=temp_out/

echo "Copying kernel image and configuration..."
mkdir -p ../osboot/
cp temp_out/arch/x86/boot/bzImage ../osboot/
cp temp_out/.config ../

echo "Cleaning up..."
rm -rf temp_out/
cd ..
