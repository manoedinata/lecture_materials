#!/usr/bin/env bash

TIMESTAMP=$(date +"%d%m%Y-%H%M%S")
OUTPUT_DIR="osboot"
BACKUP_FILE="${OUTPUT_DIR}/farewell_backup_${TIMESTAMP}.zip"

mkdir -p "$OUTPUT_DIR"

FILES_TO_BACKUP=(
	"bzImage"
	"single.gz"
	"multi.gz"
	"farewell.iso"
)

zip -j "$BACKUP_FILE" "${FILES_TO_BACKUP[@]/#/$OUTPUT_DIR/}" 2>/dev/null

echo "Backup created: $BACKUP_FILE"
