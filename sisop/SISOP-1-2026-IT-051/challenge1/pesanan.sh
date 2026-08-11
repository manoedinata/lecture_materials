#!/bin/bash

menu=("Nasi" "Kopi" "Teh" "Roti" "Ayam")
pesanan=${menu[$(shuf -i 0-4 -n 1)]}

echo "Membeli: $pesanan"

