Isi paketnya:

- **`Bab_02_Ringkasan_Textbook_Reconnaissance_Pasif_dan_OSINT.md`** — teori pasif recon & OSINT, ditulis mengalir dengan analogi (WHOIS, Google Dorking, Shodan/Censys/Netcraft/crt.sh, theHarvester, kebocoran dari GitHub, security header/TLS, batasan LLM untuk OSINT).
- **`Bab_02_Modul_Praktikum_OSINT_Pasif.md`** — praktikum dua bagian: **Bagian A** (tools OSINT sungguhan terhadap domain yang sah diriset — WHOIS, dig, crt.sh, theHarvester, Shodan) dan **Bagian B** (hands-on lokal, aman, tanpa risiko etika).
- **`resources/`**:
    - Website fiktif **"PT Contoh Nusantara"** (`www/`) — lengkap dengan jebakan OSINT realistis: komentar HTML berisi kredensial staging lama, meta tag CMS, `robots.txt` yang membocorkan path tersembunyi, halaman direktori staf internal, dan file backup `.bak` yang lupa dihapus.
    - `docker-compose.yml` + `nginx-default.conf` untuk menjalankan target itu di `localhost:8080`.
    - `Vagrantfile` + `provision.sh` — provisioning Kali dengan `whois`, `dig`, `theHarvester`, `exiftool`, dan Docker.
    - `osint_recon.sh` — automasi WHOIS/DNS/theHarvester untuk Bagian A.
    - `template_osint_mindmap.md` — template dokumentasi hasil riset.
    - `README.md` — panduan lengkap plus spoiler jawaban tersembunyi (`<details>`) untuk validasi setelah mahasiswa mencoba sendiri.