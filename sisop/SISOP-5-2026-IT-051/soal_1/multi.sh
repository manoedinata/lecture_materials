#!/usr/bin/env bash

# biar gampang
OUTPUT_DIR="osboot"
OUTPUT_FILE="$OUTPUT_DIR/multi.gz"
STAGING_DIR="rootfs_multi"

echo ""
echo "=== Multi-User File System ==="
echo ""

if [ "$EUID" -ne 0 ]; then
	echo "Please run as root (gunakan sudo)"
	exit 1
fi

echo "Cleaning up..."
rm -rf $STAGING_DIR
rm -f $OUTPUT_FILE

echo "Setting up filesystem structure..."
for dir in bin dev proc sys etc tmp root home/henn home/hann home/viii home/kids; do
	echo "  Creating directory: $dir/"
	mkdir -p "$STAGING_DIR/$dir"
done

echo "Scaffolding Busybox..."
cp /usr/bin/busybox $STAGING_DIR/bin/busybox
chmod +x $STAGING_DIR/bin/busybox

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

echo "Creating init script..."

cat <<'EOF' >$STAGING_DIR/init
#!/bin/sh
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

# Login process
while true; do
	setsid cttyhack /bin/login
done
EOF
sed -i 's/\r$//' $STAGING_DIR/init
chmod +x $STAGING_DIR/init

cat <<'EOF' >$STAGING_DIR/etc/profile
clear
cat << 'BANNER'
  ______                          _ _  ____   _____ 
 |  ____|                        | | |/ __ \ / ____|
 | |__ __ _ _ __ _____      _____| | | |  | | (___  
 |  __/ _` | '__/ _ \ \ /\ / / _ \ | | |  | |\___ \ 
 | | | (_| | | |  __/\ V  V /  __/ | | |__| |____) |
 |_|  \__,_|_|  \___| \_/\_/ \___|_|_|\____/|_____/ 

BANNER
echo ""
echo "Welcome, $(whoami)."
echo ""
EOF
chmod 644 $STAGING_DIR/etc/profile

echo "Configuring hostname..."
echo "farewell" >$STAGING_DIR/etc/hostname

echo "Configuring users..."

PW_ROOT=$(openssl passwd -1 root123)
PW_HENN=$(openssl passwd -1 henn123)
PW_HANN=$(openssl passwd -1 hann123)
PW_VIII=$(openssl passwd -1 viii123)
PW_KIDS=$(openssl passwd -1 kids123)

cat <<EOF >$STAGING_DIR/etc/passwd
root:x:0:0:root:/root:/bin/sh
henn:x:1000:1000:henn:/home/henn:/bin/sh
hann:x:1001:1001:hann:/home/hann:/bin/sh
viii:x:1002:1002:viii:/home/viii:/bin/sh
kids:x:1003:1003:kids:/home/kids:/bin/sh
EOF

cat <<EOF >$STAGING_DIR/etc/shadow
root:${PW_ROOT}:19000:0:99999:7:::
henn:${PW_HENN}:19000:0:99999:7:::
hann:${PW_HANN}:19000:0:99999:7:::
viii:${PW_VIII}:19000:0:99999:7:::
kids:${PW_KIDS}:19000:0:99999:7:::
EOF

echo "Configuring groups..."
cat <<EOF >$STAGING_DIR/etc/group
root:x:0:
henn:x:1000:
hann_access:x:1001:hann,henn
viii_access:x:1002:viii,hann,henn
kids_access:x:1003:kids,viii,hann,henn
EOF

echo "Setting permissions for files and directories..."

# Default: Selain spesifikasi khusus, direktori umum hanya r-x (Read & Execute)
chmod 755 $STAGING_DIR/{bin,dev,proc,sys,etc,home}
# /tmp Full Akses
chmod 1777 $STAGING_DIR/tmp
# /root Full Akses (Hanya root)
chown -R 0:0 $STAGING_DIR/root && chmod 700 $STAGING_DIR/root

# /home/henn: Hanya henn (700)
chown -R 1000:1000 $STAGING_DIR/home/henn && chmod 700 $STAGING_DIR/home/henn
# /home/hann: owner hann, group hann_access (yg isinya hann & henn). Others ditolak (770)
chown -R 1001:1001 $STAGING_DIR/home/hann && chmod 770 $STAGING_DIR/home/hann
# /home/viii: owner viii, group viii_access (yg isinya viii, hann, henn). Others ditolak (770)
chown -R 1002:1002 $STAGING_DIR/home/viii && chmod 770 $STAGING_DIR/home/viii
# /home/kids: owner kids, group kids_access (yg isinya kids, viii, hann, henn). Others ditolak (770)
chown -R 1003:1003 $STAGING_DIR/home/kids && chmod 770 $STAGING_DIR/home/kids

echo "Packing filesystem into $OUTPUT_FILE..."
mkdir -p $OUTPUT_DIR
cd $STAGING_DIR
# Mencari semua file, dipack dengan cpio, lalu di-compress dengan gzip tingkat 9 (maksimal)
find . -print0 | cpio --null -ov --format=newc 2>/dev/null | gzip -9 >../$OUTPUT_FILE
cd ..

echo "Cleaning up..."
rm -rf $STAGING_DIR

echo "Done! File: $OUTPUT_FILE"
