#!/bin/bash

# 1. BUAT DIREKTORI
mkdir -p /libraryit/ebooks /libraryit/papers /libraryit/sourcecode /libraryit/docs

# 2. UBAH KEPEMILIKAN
chown root:staff /libraryit/ebooks /libraryit/papers /libraryit/sourcecode /libraryit/docs

# 3. ATUR PERMISSION DASAR (Gunakan 555 untuk docs agar memicu permission denied di host)
chmod 750 /libraryit/sourcecode
chmod 775 /libraryit/ebooks
chmod 775 /libraryit/papers
chmod 555 /libraryit/docs

# 4. ATUR ACL LIBRARIAN
setfacl -m u:1002:rwx /libraryit/docs

# 5. SETUP SISTEM LOG
touch /tmp/audit.log
touch /logs/libraryit.log

rsyslogd

# 6. INJECT SCRIPT PARSER PYTHON
cat <<'EOF' >/tmp/logger.py
import time, datetime, os

log_file = '/tmp/audit.log'
out_file = '/logs/libraryit.log'

with open(log_file, 'r') as f, open(out_file, 'a') as out:
    f.seek(0, 2)
    while True:
        line = f.readline()
        if not line:
            time.sleep(0.1)
            continue
        
        if "AUDIT" not in line: continue
        
        try:
            raw_data = line.split("AUDIT")[1].strip()
            if raw_data.startswith("|"):
                raw_data = raw_data[1:]
                
            parts = [p.strip() for p in raw_data.split('|') if p.strip()]
            if len(parts) < 4: continue
            
            user = parts[0]
            share = parts[1]
            action = parts[2].upper()
            status = parts[3]
            filename = parts[4].split('/')[-1] if len(parts) > 4 else share

            level = "INFO"
            aksi = action
            target = filename

            if "fail" in status.lower() or "denied" in status.lower() or "err" in status.lower():
                level = "WARNING"
                aksi = "DENIED"
                target = share
            elif action == "PWRITE":
                aksi = "WRITE"
            elif action == "CONNECT":
                aksi = "CONNECT"
                target = share
            else:
                continue

            ts = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            log_line = f"[{ts}] [{level}] [{user}] [{aksi}] [{target}]\n"
            
            out.write(log_line)
            out.flush()
        except Exception as e:
            continue
EOF

# 7. EKSEKUSI LOGGER DI BACKGROUND
python3 /tmp/logger.py &

# 8. JALANKAN DAEMON SAMBA UTAMA
exec smbd --foreground --no-process-group
