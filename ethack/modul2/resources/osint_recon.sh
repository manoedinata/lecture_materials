#!/usr/bin/env bash
# osint_recon.sh - Bab 2: Automasi sebagian langkah OSINT pasif (Bagian A)
#
# Cara pakai:
#   chmod +x osint_recon.sh
#   ./osint_recon.sh <domain-target-yang-sah>
#
# Contoh:
#   ./osint_recon.sh contoh-domain-sah.com
#
# PENTING: hanya gunakan terhadap domain yang memang sah untuk kamu riset
# (domain sendiri, atau domain dengan program bug bounty publik yang jelas).

DOMAIN="$1"
OUTDIR="$HOME/Lab-Notes/Bab-02-OSINT"
TIMESTAMP=$(date +"%Y%m%d-%H%M%S")

if [ -z "$DOMAIN" ]; then
    echo "Penggunaan: ./osint_recon.sh <domain-target-yang-sah>"
    exit 1
fi

mkdir -p "$OUTDIR"

echo "=============================================="
echo " OSINT Pasif terhadap: $DOMAIN"
echo " (Pastikan kamu punya izin sah untuk riset ini)"
echo "=============================================="

echo ""
echo "[1/3] WHOIS lookup..."
whois "$DOMAIN" | tee "${OUTDIR}/whois-${TIMESTAMP}.txt"

echo ""
echo "[2/3] DNS record (MX, NS, A)..."
{
    echo "--- MX Records ---"
    dig "$DOMAIN" MX +short
    echo "--- NS Records ---"
    dig "$DOMAIN" NS +short
    echo "--- A Record ---"
    dig "$DOMAIN" A +short
} | tee "${OUTDIR}/dns-${TIMESTAMP}.txt"

echo ""
echo "[3/3] theHarvester (email & subdomain dari sumber publik)..."
theHarvester -d "$DOMAIN" -b all -l 200 -f "${OUTDIR}/theharvester-${TIMESTAMP}" || \
    echo "    (Beberapa sumber theHarvester mungkin butuh API key - lihat dokumentasi resminya)"

echo ""
echo "[*] Selesai. Jangan lupa cek juga secara manual:"
echo "    - https://crt.sh/?q=%25.${DOMAIN}  (Certificate Transparency)"
echo "    - https://www.shodan.io (butuh akun gratis)"
echo "    - https://www.netcraft.com/apps/report_template/index.html"
echo ""
echo "[*] Pindahkan seluruh temuan ke template_osint_mindmap.md"
