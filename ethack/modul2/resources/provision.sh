#!/usr/bin/env bash
# provision.sh - Bab 2: Setup tools OSINT di dalam Kali VM
# Dijalankan otomatis oleh Vagrant, atau manual dengan:
#   chmod +x provision.sh && sudo ./provision.sh

set -e

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
      $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
      tee /etc/apt/sources.list.d/docker.list > /dev/null
    apt-get update -y
    apt-get install -y docker-ce docker-ce-cli containerd.io docker-compose-plugin
else
    echo "    Docker sudah terpasang, dilewati."
fi

usermod -aG docker vagrant || true

echo "[*] Membuat struktur folder catatan lab Bab 2..."
sudo -u vagrant mkdir -p /home/vagrant/Lab-Notes/Bab-02-OSINT/screenshot
sudo -u vagrant touch /home/vagrant/Lab-Notes/Bab-02-OSINT/00-scope-dan-tujuan.md
sudo -u vagrant touch /home/vagrant/Lab-Notes/Bab-02-OSINT/01-osint-notes.md

echo "[*] Selesai! Jalankan 'docker compose up -d' di folder resource untuk menghidupkan target PT Contoh Nusantara."
echo "[*] Lalu gunakan osint_recon.sh untuk automasi sebagian langkah OSINT."
