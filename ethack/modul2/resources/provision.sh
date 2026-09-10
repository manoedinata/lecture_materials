#!/usr/bin/env bash
# provision.sh - Bab 2: Setup tools OSINT di dalam Kali VM
# Dijalankan otomatis oleh Vagrant, atau manual dengan:
#   chmod +x provision.sh && sudo ./provision.sh

set -e

# Deteksi user lab. Di VM hasil "vagrant up" ini akan bernilai "vagrant" (user
# bawaan box kalilinux/rolling). Kalau script dijalankan manual dengan sudo di
# Kali VM biasa (Bab 1), nilainya mengikuti user yang memanggil sudo, mis. "kali".
LAB_USER="${SUDO_USER:-$(id -nu 1000 2>/dev/null || echo root)}"
LAB_HOME="$(getent passwd "$LAB_USER" | cut -d: -f6)"
LAB_HOME="${LAB_HOME:-/root}"
echo "[*] User lab terdeteksi: $LAB_USER (home: $LAB_HOME)"

# Kali rolling adalah turunan Debian testing dan VERSION_CODENAME-nya bernilai
# "kali-rolling" -- suite yang tidak pernah diterbitkan Docker. Jadi codename
# Debian dipilih manual. Override kalau perlu:
#   DOCKER_DEB_CODENAME=trixie sudo -E ./provision.sh
DOCKER_DEB_CODENAME="${DOCKER_DEB_CODENAME:-bookworm}"
DOCKER_LIST=/etc/apt/sources.list.d/docker.list

# Buang sisa repo Docker yang salah suite (mis. kali-rolling dari versi script
# lama), supaya apt-get update di bawah tidak langsung gagal.
if [ -f "$DOCKER_LIST" ] && ! grep -q " $DOCKER_DEB_CODENAME " "$DOCKER_LIST"; then
    echo "[!] Repo Docker lama dengan suite tidak valid ditemukan, dihapus: $DOCKER_LIST"
    rm -f "$DOCKER_LIST"
fi

echo "[*] Update package list..."
apt-get update -y

echo "[*] Memasang tools OSINT dasar (whois, dnsutils/dig, exiftool)..."
apt-get install -y whois dnsutils libimage-exiftool-perl

echo "[*] Memastikan theHarvester terpasang (biasanya sudah ada di Kali)..."
if ! command -v theHarvester &> /dev/null; then
    apt-get install -y theharvester
else
    echo "    theHarvester sudah terpasang, dilewati."
fi

echo "[*] Memasang Docker Engine (untuk menjalankan target 'PT Contoh Nusantara')..."
if ! command -v docker &> /dev/null; then
    apt-get install -y ca-certificates curl gnupg
    install -m 0755 -d /etc/apt/keyrings
    curl -fsSL https://download.docker.com/linux/debian/gpg -o /etc/apt/keyrings/docker.asc
    chmod a+r /etc/apt/keyrings/docker.asc
    echo \
      "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/debian \
      $DOCKER_DEB_CODENAME stable" | \
      tee "$DOCKER_LIST" > /dev/null
    apt-get update -y
    apt-get install -y docker-ce docker-ce-cli containerd.io docker-compose-plugin
else
    echo "    Docker sudah terpasang, dilewati."
fi

usermod -aG docker "$LAB_USER" || true

echo "[*] Membuat struktur folder catatan lab Bab 2..."
NOTES_DIR="$LAB_HOME/Lab-Notes/Bab-02-OSINT"
sudo -u "$LAB_USER" mkdir -p "$NOTES_DIR/screenshot"
sudo -u "$LAB_USER" touch "$NOTES_DIR/00-scope-dan-tujuan.md"
sudo -u "$LAB_USER" touch "$NOTES_DIR/01-osint-notes.md"

echo "[*] Selesai! Jalankan 'docker compose up -d' di folder resource untuk menghidupkan target PT Contoh Nusantara."
echo "[*] Lalu gunakan osint_recon.sh untuk automasi sebagian langkah OSINT."
