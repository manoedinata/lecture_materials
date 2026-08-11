FROM debian:bookworm-slim

# Install dependencies
RUN apt-get update && \
    apt-get install -y samba acl rsyslog python3 && \
    rm -rf /var/lib/apt/lists/*

# Nonaktifkan modul imklog untuk mencegah log audit Samba masuk ke syslog utama
RUN sed -i '/imklog/s/^/#/' /etc/rsyslog.conf

# Tambah user
RUN useradd -m -u 1000 -s /bin/bash member && \
    useradd -m -u 1001 -s /bin/bash contributor && \
    useradd -m -u 1002 -s /bin/bash librarian

# Tambah grup (Grup 'staff' sudah ada di Debian (SystemGroup))
RUN groupadd readonly && \
    usermod -aG readonly member && \
    usermod -aG staff contributor && \
    usermod -aG staff librarian

# Autentikasi samba
RUN (echo "member123"; echo "member123") | smbpasswd -s -a member && \
    (echo "contrib456"; echo "contrib456") | smbpasswd -s -a contributor && \
    (echo "lib789"; echo "lib789") | smbpasswd -s -a librarian

# Konfigurasi rsyslog agar membuang log audit Samba langsung ke file /tmp/audit.log
RUN echo "local5.* /tmp/audit.log" > /etc/rsyslog.d/samba-audit.conf

# Copy Konfigurasi Samba & Entrypoint
COPY smb.conf /etc/samba/smb.conf
COPY entrypoint.sh /entrypoint.sh

RUN chmod +x /entrypoint.sh

CMD ["/entrypoint.sh"]
