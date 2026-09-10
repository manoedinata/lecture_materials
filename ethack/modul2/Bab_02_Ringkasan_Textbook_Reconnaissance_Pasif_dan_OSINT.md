# Bab 2 — Reconnaissance Pasif & OSINT

Ada kebiasaan buruk yang hampir semua pemula pentest lakukan tanpa sadar: begitu dapat target, langsung buka terminal, langsung `nmap -A`, langsung gaspol. Padahal, penyerang (dan pentester profesional) yang benar-benar hati-hati justru menghabiskan waktu jauh lebih lama di tahap yang bahkan belum menyentuh satu paket pun ke jaringan target.

Tahap itu namanya reconnaissance pasif. Dan jujur saja, ini salah satu keahlian yang paling underrated di dunia keamanan siber — padahal dari sinilah biasanya muncul temuan-temuan yang bikin klien kaget: "Lho, kok kalian bisa tahu nama karyawan kami cuma dari internet?"

## 2.1 Pasif vs Aktif: Kenapa Urutan Ini Penting

Bedanya sederhana kalau dijelaskan pakai analogi: reconnaissance pasif itu seperti kamu mengamati sebuah rumah dari kejauhan — lihat berapa mobil yang parkir, baca nama di kotak pos, cek data kepemilikan tanah di kantor kelurahan. Semua ini bisa kamu lakukan tanpa pemilik rumah tahu sama sekali kalau sedang diamati. Reconnaissance aktif, sebaliknya, seperti kamu mengetuk pintu, mencoba gagang pintu, atau mengintip lewat jendela — pemilik rumah punya kesempatan untuk sadar bahwa ada orang asing yang beraktivitas di sekitar propertinya.

Kenapa urutannya harus pasif dulu baru aktif? Dua alasan utama. Pertama, alasan taktis: semakin banyak informasi yang kamu kumpulkan tanpa menyentuh sistem target, semakin efisien dan terarah tahap aktif nantinya — kamu nggak perlu scan buta ke seluruh rentang IP kalau kamu sudah tahu persis subdomain mana saja yang aktif. Kedua, alasan operasional: aktivitas pasif praktis tidak meninggalkan jejak di log target, sementara aktivitas aktif (apalagi kalau ceroboh) bisa memicu alert di sistem monitoring mereka jauh sebelum kamu sempat menemukan apapun yang berguna.

## 2.2 Mengenal Anatomi Sebuah Domain

Sebelum masuk ke tools, ada baiknya kamu paham dulu struktur dasar yang bakal terus muncul sepanjang bab ini.

Setiap domain terdaftar di sebuah **registrar** (perusahaan yang menjual domain, seperti Namecheap atau GoDaddy), dan informasi registrasinya tersimpan di database **WHOIS** — meski belakangan ini banyak registrar menyembunyikan data pemilik demi privasi (GDPR turut berperan besar di sini, jadi jangan kaget kalau hasil WHOIS zaman sekarang seringnya cuma menunjukkan data registrar, bukan pemilik asli).

Domain juga punya **name server**, yaitu server yang bertanggung jawab menerjemahkan nama domain jadi alamat IP. Dan yang sering luput dari perhatian pemula: satu organisasi biasanya punya banyak **subdomain** — bukan cuma `www.perusahaan.com`, tapi bisa juga `mail.`, `dev.`, `staging.`, `vpn.`, `admin.`, dan seterusnya. Subdomain yang "lupa" dinonaktifkan atau masih pakai konfigurasi development inilah yang sering jadi pintu masuk paling mudah — bukan karena canggih secara teknis, tapi karena memang lolos dari perhatian tim IT.

## 2.3 WHOIS dan Google Dorking: Senjata Paling Klasik

Mulai dari yang paling dasar dulu. Command `whois namadomain.com` akan menampilkan info registrar, tanggal registrasi, dan kadang-kadang (kalau belum di-privasi-kan) kontak administratif. Meski hasilnya makin sering "disensor", tanggal registrasi dan history perubahan tetap berguna — domain yang baru didaftarkan minggu lalu, misalnya, bisa jadi indikasi campaign phishing yang sedang berjalan (ini justru lebih sering dipakai defender untuk deteksi, tapi prinsipnya sama).

Nah, yang jauh lebih powerful sebenarnya **Google Dorking** — teknik memanfaatkan operator pencarian lanjutan Google untuk menemukan informasi yang "nggak sengaja" ter-index tapi sebenarnya tidak dimaksudkan untuk publik. Beberapa operator yang wajib kamu kuasai:

- `site:` — membatasi pencarian hanya ke satu domain. Contoh: `site:perusahaan.com filetype:pdf` untuk cari semua dokumen PDF yang ter-index dari domain tersebut.
- `inurl:` — mencari string tertentu di URL, misalnya `inurl:admin` untuk mencari halaman admin yang ter-index tanpa sengaja.
- `intitle:` — mencari berdasarkan judul halaman.
- `filetype:` — membatasi hasil ke jenis file tertentu (PDF, XLS, DOC, dan sebagainya — dokumen internal yang bocor sering ditemukan lewat cara ini).

Kalau kamu ingin belajar kombinasi-kombinasi dork yang sudah terbukti efektif, **Google Hacking Database (GHDB)** di Exploit-DB adalah kumpulan crowdsourced dari komunitas selama bertahun-tahun. Isinya mulai dari dork untuk menemukan kamera CCTV yang expose ke internet sampai file konfigurasi yang bocor kredensialnya.

## 2.4 Framework OSINT: Bukan Cuma Google

Setelah paham dasar, saatnya kenalan dengan tools yang lebih terstruktur.

**Shodan** sering disebut "Google-nya perangkat internet" — bukan mengindeks halaman web, tapi mengindeks perangkat dan service yang terhubung ke internet: server, router, kamera IP, bahkan sistem SCADA industri yang harusnya nggak boleh terekspos sama sekali. Dengan akun gratis, kamu sudah bisa cari perangkat berdasarkan IP, organisasi, atau bahkan banner service tertentu.

**Censys** mirip filosofinya dengan Shodan, kadang memberikan data pelengkap yang tidak ditemukan di Shodan atau sebaliknya — kebiasaan baik untuk cross-check hasil di lebih dari satu sumber sebelum menyimpulkan sesuatu.

**Netcraft** berguna untuk melihat riwayat hosting sebuah domain — kapan pindah provider, teknologi apa yang pernah dan sedang dipakai. Ini kadang membantu memahami "jejak sejarah" infrastruktur yang bisa jadi masih meninggalkan sisa konfigurasi lama.

**crt.sh** adalah pencarian **Certificate Transparency log** — setiap kali organisasi menerbitkan sertifikat TLS/SSL (termasuk untuk subdomain internal sekalipun), catatannya tercatat publik di log ini. Ini salah satu cara paling efektif untuk menemukan subdomain yang mungkin tidak pernah kamu temukan lewat brute-force biasa, karena datanya berasal langsung dari proses penerbitan sertifikat, bukan tebakan.

**theHarvester** adalah tool command-line yang mengagregasi banyak sumber sekaligus — email, subdomain, nama karyawan — dari mesin pencari publik dan API layanan OSINT lainnya, dalam satu perintah.

**Maltego Community Edition** memberikan pendekatan visual: kamu bisa memetakan relasi antar entitas (domain, email, orang, perusahaan) dalam bentuk graph interaktif, yang jauh lebih mudah dibaca ketimbang daftar teks panjang, terutama untuk target dengan jaringan relasi yang kompleks.

## 2.5 OSINT dari Kode Sumber Terbuka

Bagian ini sering jadi kejutan buat pemula: ternyata banyak sekali kebocoran informasi sensitif justru berasal dari repository kode yang sengaja atau tidak sengaja dipublikasikan di GitHub.

Developer, sebagaimana manusia pada umumnya, kadang lupa. Commit history yang menyimpan API key lama, file `.env` yang ter-commit tanpa sengaja, atau bahkan komentar kode yang mengungkap arsitektur internal — semua ini bisa ditemukan lewat teknik **GitHub dorking** (mirip Google dorking, tapi khusus untuk mesin pencari GitHub) atau tools otomatis seperti **truffleHog** dan **gitleaks** yang dirancang khusus untuk memindai history commit mencari pola-pola seperti API key, private key, atau token akses yang bocor.

Menariknya, kebocoran semacam ini seringkali *tidak bisa dihapus* sepenuhnya hanya dengan menghapus file dari commit terbaru — selama history git masih menyimpan versi lama, data itu masih bisa digali. Ini pelajaran penting bukan cuma buat attacker, tapi juga developer: sekali credential ter-commit, anggap saja sudah bocor permanen, ganti credentialnya, jangan cuma dihapus dari commit.

## 2.6 Membaca Security Header dan Sertifikat TLS

Ini teknik yang agak lebih halus tapi cukup powerful untuk inferensi arsitektur backend tanpa perlu tools canggih — cukup lihat response header HTTP dari browser atau `curl -I`.

Header seperti `Content-Security-Policy` (CSP), `Strict-Transport-Security` (HSTS), dan `X-Frame-Options` bukan cuma indikator seberapa serius tim keamanan organisasi tersebut menangani hardening — kadang isinya sendiri membocorkan informasi. CSP misalnya, sering mendaftar domain-domain eksternal yang dipercaya (CDN, layanan analytics, API pihak ketiga), yang secara tidak langsung memberimu peta tentang teknologi apa saja yang mereka pakai.

Begitu juga dengan sertifikat TLS — Subject Alternative Name (SAN) pada sertifikat sering mencantumkan banyak subdomain sekaligus dalam satu sertifikat wildcard, memberimu daftar subdomain "gratis" tanpa perlu brute force sama sekali.

## 2.7 Automasi dan Peran (serta Batasan) AI dalam OSINT

Kalau kamu sudah terbiasa manual, saatnya kenalan dengan automasi. **SpiderFoot** (open-source) bisa menjalankan puluhan modul OSINT sekaligus secara otomatis — dari WHOIS, DNS, sampai pencarian breach data — dan menyajikannya dalam satu dashboard terintegrasi. Sangat membantu untuk riset skala besar, tapi jangan sampai kamu jadi malas memvalidasi manual hasil-hasilnya.

Soal AI/LLM, ini topik yang lagi hangat-hangatnya. Memang benar, LLM bisa sangat membantu mensintesis data OSINT dalam jumlah besar jadi ringkasan yang mudah dicerna — misalnya kamu lempar puluhan halaman hasil scraping, minta dirangkum jadi profil organisasi. Tapi ingat baik-baik keterbatasannya: LLM bisa berhalusinasi (mengarang fakta yang kedengarannya meyakinkan tapi salah), dan data pelatihannya punya batas waktu tertentu sehingga bisa saja memberi informasi yang sudah basi. Selalu perlakukan output LLM sebagai *starting point* untuk verifikasi, bukan sebagai kebenaran final.

## Rangkuman Bab

- Reconnaissance pasif dilakukan dulu, sebelum aktif — demi efisiensi dan demi menghindari jejak dini di log target.
- WHOIS dan Google Dorking adalah teknik paling dasar tapi tetap relevan sampai sekarang.
- Shodan, Censys, Netcraft, dan crt.sh masing-masing punya sudut pandang berbeda — biasakan cross-check, jangan puas dengan satu sumber saja.
- Kode sumber terbuka (GitHub dkk.) adalah sumber kebocoran informasi yang sering diremehkan.
- Security header dan sertifikat TLS bisa dibaca untuk inferensi arsitektur tanpa menyentuh sistem sama sekali.
- AI/LLM membantu sintesis, tapi tetap butuh validasi manusia.

## Checkpoint Mandiri

1. Bisakah kamu membedakan mana aktivitas yang benar-benar pasif dan mana yang sebenarnya sudah masuk kategori aktif (menyentuh sistem target)?
2. Kenapa Certificate Transparency log (crt.sh) sering lebih efektif menemukan subdomain dibanding brute-force wordlist biasa?
3. Bisakah kamu jelaskan kenapa menghapus file `.env` dari commit terbaru saja tidak cukup untuk mengamankan credential yang sudah ter-commit?

## Tantangan Mastery

Pilih satu domain publik yang secara sah boleh kamu riset (domain milikmu sendiri, atau domain yang memiliki program bug bounty publik dengan izin riset OSINT eksplisit). Susun profil attack-surface lengkap — mencakup subdomain, teknologi yang terdeteksi, dan potensi pola email organisasi — **tanpa mengirim satu paket pun secara langsung ke server target** (semua sumber harus dari layanan pihak ketiga seperti crt.sh, Shodan, atau mesin pencari).

## Sumber Bacaan Lanjutan

- OSINT Framework (kumpulan tools terkategorisasi): https://osintframework.com/
- Google Hacking Database: https://www.exploit-db.com/google-hacking-database
- crt.sh — Certificate Transparency Search: https://crt.sh/
- Dokumentasi resmi theHarvester: https://github.com/laramies/theHarvester
- Dokumentasi resmi SpiderFoot: https://www.spiderfoot.net/
