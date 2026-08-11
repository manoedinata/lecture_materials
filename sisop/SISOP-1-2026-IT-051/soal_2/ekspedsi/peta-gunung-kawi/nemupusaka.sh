#!/bin/bash

FILE=$1
if [ -z "$FILE" ]; then
    FILE="titik-penting.txt"
fi

# Disini, diambil titik 1 dan titik 3 karena berseberangan.
lat1=$(sed -n '1p' "$FILE" | cut -d',' -f3)
lat3=$(sed -n '3p' "$FILE" | cut -d',' -f3)

lon1=$(sed -n '1p' "$FILE" | cut -d',' -f4)
lon3=$(sed -n '3p' "$FILE" | cut -d',' -f4)

lat_tengah=$(awk "BEGIN {printf \"%.6f\", ($lat1 + $lat3) / 2}")
lon_tengah=$(awk "BEGIN {printf \"%.6f\", ($lon1 + $lon3) / 2}")

echo "Koordinat pusat:"
echo "$lat_tengah,$lon_tengah"

echo "$lat_tengah,$lon_tengah" > posisipusaka.txt
