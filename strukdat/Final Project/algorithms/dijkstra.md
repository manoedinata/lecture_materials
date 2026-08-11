# Dijkstra's Algorithm

## Konsep Dasar
Algoritma Dijkstra digunakan untuk mencari rute terpendek (atau dalam kasus ini, termurah/tercepat) dari satu stasiun asal (*single-source*) ke semua stasiun lain dalam graf berbobot positif.

Pada proyek transportasi ini, Dijkstra digunakan untuk dua fitur utama:
1. **Rute Tercepat** (`MODE_TIME`): Menggunakan waktu tempuh (menit) sebagai bobot graf.
2. **Rute Termurah** (`MODE_COST`): Menggunakan biaya (Rupiah) sebagai bobot graf.

## Struktur Data Pendukung
- **Array `dist[]`**: Menyimpan total jarak (waktu/biaya) minimum sementara dari stasiun asal ke stasiun indeks `i`. Diinisialisasi dengan `Integer.MAX_VALUE` ($\infty$).
- **Array `parent[]`**: Menyimpan ID stasiun sebelumnya (*backtrack pointer*) untuk merangkai jalur stasiun (path).
- **Array `visited[]`**: Menandai stasiun yang sudah dikunjungi dan ditemukan jarak terpendek fix-nya agar tidak diproses berulang.
- **Min-Heap (Priority Queue)**: Struktur data kustom untuk mengambil stasiun dengan jarak terpendek saat ini secara efisien.

## Mekanisme Cara Kerja
1. **Inisialisasi**: Atur `dist[source] = 0`. Masukkan node `source` ke dalam Min-Heap dengan prioritas $0$.
2. **Iterasi Utama**: Selama Min-Heap tidak kosong:
   - **Extract-Min**: Ambil node `u` dengan nilai `dist` terkecil dari heap.
   - Jika `u` sudah dikunjungi (`visited[u] == true`), abaikan (lanjut ke elemen heap berikutnya).
   - Tandai `visited[u] = true`.
   - **Early Exit**: Jika `u` adalah `destId` (stasiun tujuan), pencarian langsung dihentikan (*break*) untuk menghemat waktu komputasi.
3. **Relaksasi (Edge Relaxation)**: Untuk setiap tetangga `v` dari node `u`:
   - Hitung jarak baru: `newDist = dist[u] + weight(u, v)`.
   - (Catatan: `weight` ditentukan oleh parameter `mode`, apakah waktu atau biaya).
   - Jika `newDist < dist[v]`, maka:
     - *Update* `dist[v] = newDist`.
     - *Update* `parent[v] = u`.
     - Lakukan penyisipan (`insert`) atau perbaruan posisi (`decreaseKey`) ke dalam Min-Heap.
4. **Rekonstruksi Jalur**: Telusuri balik dari `destId` menggunakan array `parent[]` hingga mencapai node awal, kemudian balik (reverse) urutannya untuk menghasilkan *list path* rute dari awal hingga akhir.

## Kompleksitas
Dengan menggunakan Min-Heap kustom yang memiliki relasi `decreaseKey` $O(\log V)$, maka algoritma berjalan pada kompleksitas:
**$O((V + E) \log V)$** 
di mana $V$ adalah jumlah node (Stasiun) dan $E$ adalah jumlah edge (Rute).