#!/bin/bash

OUTPUT_FILE="titik-penting.txt"

FILE=$1
if [ -z "$FILE" ]; then
    # echo "Usage: $0 <file>"
    # exit 1
    FILE="gsxtrack.json"
fi

# Flow:
# 1. Ambil baris yang mengandung "id", "site_name", "latitude", atau "longitude" menggunakan grep
# 2. Gunakan sed untuk membersihkan dan memformat output menjadi "key: value"
# 3. Gunakan awk untuk menggabungkan informasi yang terkait (id, site_name, latitude, longitude) dan mencetaknya dalam format yang rapi

grep -E '"id":|"site_name":|"latitude":|"longitude":' $FILE | \
sed -E 's/^[[:space:]]*"(id|site_name|latitude|longitude)": "?([^",]+)"?,?.*/\1: \2/' | \
awk -F': ' '
  /^id:/ { id=$2 }
  /^site_name:/ { site=$2 }
  /^latitude:/ { lat=$2 }
  /^longitude:/ { lon=$2; printf "%s,%s,%s,%s\n", id, site, lat, lon }
' > $OUTPUT_FILE

echo "Koordinat titik penting telah disimpan di $OUTPUT_FILE"
