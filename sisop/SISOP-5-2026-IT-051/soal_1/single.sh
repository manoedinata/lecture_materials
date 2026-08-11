#!/usr/bin/env bash

# biar gampang
OUTPUT_DIR="osboot"
OUTPUT_FILE="$OUTPUT_DIR/single.gz"
STAGING_DIR="rootfs_temp"

echo ""
echo "=== Single-User File System ==="
echo ""

if [ "$EUID" -ne 0 ]; then
	echo "Please run as root (gunakan sudo)"
	exit 1
fi

echo "Cleaning up..."
rm -rf $STAGING_DIR
rm -f $OUTPUT_FILE

echo "Setting up filesystem structure..."
for dir in bin dev proc sys etc tmp root; do
	echo "  Creating directory: $dir/"
	mkdir -p "$STAGING_DIR/$dir"
done

echo "Scaffolding Busybox..."
cp /usr/bin/busybox $STAGING_DIR/bin/busybox
chmod +x $STAGING_DIR/bin/busybox

echo "  Installing Busybox applets..."
cd $STAGING_DIR/bin
./busybox --install .
cd ../..

echo "Setting up Alpine package manager (apk-tools)..."

wget -q -O apk-tools.apk https://dl-cdn.alpinelinux.org/alpine/latest-stable/main/x86_64/apk-tools-static-3.0.6-r0.apk
tar -xzf apk-tools.apk sbin/apk.static && rm -f apk-tools.apk

# mv sbin/apk.static $STAGING_DIR/bin/party && rm -rf sbin

mv sbin/apk.static $STAGING_DIR/bin/ && rm -rf sbin
cat <<'EOF' >$STAGING_DIR/bin/party
#!/bin/sh
# Eksekusi apk dengan flag --allow-untrusted agar tidak rewel soal GPG keys
exec /bin/apk.static --allow-untrusted "$@"
EOF
chmod +x $STAGING_DIR/bin/party

mkdir -p $STAGING_DIR/etc/apk
mkdir -p $STAGING_DIR/lib/apk/db
mkdir -p $STAGING_DIR/var/cache/apk
$STAGING_DIR/bin/apk.static --root $STAGING_DIR --initdb add

echo "http://dl-cdn.alpinelinux.org/alpine/latest-stable/main" >$STAGING_DIR/etc/apk/repositories
echo "http://dl-cdn.alpinelinux.org/alpine/latest-stable/community" >>$STAGING_DIR/etc/apk/repositories

echo "Creating init script..."
cat <<'EOF' >$STAGING_DIR/init
#!/bin/sh
# Mount filesystem
mount -t proc none /proc
mount -t sysfs none /sys
mount -t devtmpfs none /dev

# Network setup
ifconfig lo 127.0.0.1 up
ifconfig eth0 10.0.2.15 netmask 255.255.255.0 up
route add default gw 10.0.2.2

mkdir -p /etc
echo "nameserver 1.1.1.1" > /etc/resolv.conf
echo "nameserver 8.8.8.8" >> /etc/resolv.conf

dmesg -n 1

while true; do
	echo "Halo! Selamat datang di FarewellOS-single!"
	setsid cttyhack /bin/sh
	echo "Session berakhir. Kembali ke shell..."
	sleep 1.5
done
EOF
sed -i 's/\r$//' $STAGING_DIR/init
chmod +x $STAGING_DIR/init

echo "Configuring root user..."
echo "root:x:0:0:root:/root:/bin/sh" >$STAGING_DIR/etc/passwd
echo "root:x:0:" >$STAGING_DIR/etc/group
chmod 700 $STAGING_DIR/root

sudo mknod -m 600 $STAGING_DIR/dev/console c 5 1

echo "Packing filesystem into $OUTPUT_FILE..."
mkdir -p $OUTPUT_DIR
cd $STAGING_DIR
# Mencari semua file, dipack dengan cpio, lalu di-compress dengan gzip tingkat 9 (maksimal)
find . -print0 | cpio --null -ov --format=newc 2>/dev/null | gzip -9 >../$OUTPUT_FILE
cd ..

echo "Cleaning up..."
rm -rf $STAGING_DIR

echo "Done! File: $OUTPUT_FILE"
