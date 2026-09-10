# Resource Praktikum — Bab 2: OSINT Pasif

Paket ini berisi semua file teknis untuk Modul Praktikum Bab 2, terbagi dua bagian sesuai modul:
**Bagian A** (riset terhadap sumber publik sungguhan) dan **Bagian B** (hands-on lokal dengan website fiktif "PT Contoh Nusantara").

## Isi Paket

| File/Folder | Fungsi |
|---|---|
| `www/` | Konten website fiktif PT Contoh Nusantara (target Bagian B) |
| `nginx-default.conf` | Konfigurasi nginx untuk menyajikan website tersebut |
| `docker-compose.yml` | Menjalankan target PT Contoh Nusantara di port 8080 |
| `Vagrantfile` | Provisioning VM Kali dengan tools OSINT |
| `provision.sh` | Script instalasi whois, dig, theHarvester, exiftool, Docker |
| `osint_recon.sh` | Automasi sebagian langkah OSINT Bagian A (WHOIS, DNS, theHarvester) |
| `template_osint_mindmap.md` | Template dokumentasi hasil riset OSINT |

## Setup Cepat (Jika Kali VM Sudah Ada dari Bab 1)

```bash
chmod +x provision.sh
sudo ./provision.sh
```

Lalu jalankan target Bagian B:

```bash
docker compose up -d
```

Akses `http://localhost:8080` dari browser.

## Setup dari Nol (Vagrant + VirtualBox)

```bash
vagrant up
vagrant ssh
cd /vagrant
docker compose up -d
```

## Menjalankan Bagian A (Riset Domain Sah)

```bash
chmod +x osint_recon.sh
./osint_recon.sh <domain-target-yang-sah>
```

**Ingat**: hanya jalankan ini terhadap domain yang memang sah kamu riset. Jangan pernah pakai script ini terhadap domain organisasi sembarangan tanpa izin — meski semua langkah di dalamnya bersifat pasif/read-only terhadap sumber pihak ketiga, tetap gunakan penilaian etis yang baik.

## Menjalankan Bagian B (Hands-On PT Contoh Nusantara)

Setelah container jalan, coba akses:
- `http://localhost:8080/` — halaman utama
- `http://localhost:8080/robots.txt`
- `http://localhost:8080/sitemap.xml`
- Lalu telusuri sendiri path-path yang menarik dari `robots.txt`

Jangan buka jawaban di bawah ini sebelum mencoba sendiri dulu ya:

<details>
<summary>Spoiler: daftar "kebocoran" yang sengaja ditanam (klik untuk buka)</summary>

- Komentar HTML di `index.html` berisi kredensial staging lama dan kontak staf IT.
- Meta tag `generator` membocorkan nama & versi CMS fiktif.
- `robots.txt` membocorkan path `/internal/` dan file `notes-lama.txt.bak`.
- `/internal/employees.html` berisi direktori staf lengkap dengan email.
- `notes-lama.txt.bak` berisi catatan internal termasuk nama VPN dan kontak eskalasi.
- Halaman karir membocorkan pola format email organisasi secara eksplisit.

</details>

## Menyalin Template

```bash
cp template_osint_mindmap.md ~/Lab-Notes/Bab-02-OSINT/02-peta-osint.md
```

## Catatan Keamanan & Etika Lab

- Website "PT Contoh Nusantara" adalah data fiktif 100% — nama, email, dan detail lain tidak merujuk ke entitas nyata manapun.
- Jangan expose port 8080 ke internet.
- Untuk Bagian A, selalu pastikan target yang kamu pilih memang sah untuk diriset secara OSINT.
