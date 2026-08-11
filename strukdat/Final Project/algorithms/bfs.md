# BFS (Breadth-First Search)

## Konsep Dasar
BFS adalah algoritma penjelajahan graf yang mengunjungi node level-demi-level secara terstruktur menyebar layaknya riak air (dari jarak 1, jarak 2, jarak 3 hop, dst). 

Dalam konteks proyek transportasi ini, **BFS dimanfaatkan untuk mencari rute dengan jumlah transit paling sedikit**.

## Mengapa Menggunakan BFS untuk Transit Minimum?
Berbeda dengan biaya atau waktu tempuh yang memiliki nilai / bobot spesifik per rute, "jumlah transit" pada dasarnya adalah "jumlah edge (rute)" yang dilewati. BFS murni akan menangani semua edge seolah-olah memiliki bobot yang persis sama, yaitu $1$. 
Oleh karena BFS mengembang berdasarkan level kedalaman (hop), kemunculan *pertama kali* dari stasiun tujuan dalam BFS sudah pasti merupakan **jalur terpendek secara jumlah lintasan/hop (minimum transits)**.

## Struktur Data Pendukung
- **Array `visited[]`**: Mencegah graf berputar dalam *cycle* dengan menandai stasiun yang telah dieksplor.
- **Array `parent[]`**: Menyimpan lintasan mundur (backtrack path).
- **Simple Queue**: Antrean FIFO (*First-In-First-Out*) yang digunakan untuk mengatur urutan kunjungan level. 

## Mekanisme Cara Kerja
1. **Inisialisasi**: Antre-kan (`enqueue`) stasiun asal ke dalam Queue dan tandai sebagai `visited`.
2. **Eksplorasi FIFO**: Selama Queue tidak kosong:
   - Keluarkan (`dequeue`) elemen terdepan `u` dari antrean.
   - Periksa jika `u == destId`. Jika ya, berhenti karena tujuan sudah tercapai di hop terkecil.
   - Iterasi ke semua tetangga dari `u`.
   - Jika tetangga `v` belum dikunjungi (`!visited[v]`):
     - Tandai `visited[v] = true`.
     - Set `parent[v] = u`.
     - Antre-kan `v` (`enqueue`) ke belakang Queue untuk diproses pada level berikutnya.
3. **Rekonstruksi Jalur**: Sama seperti Dijkstra, mundur mengikuti `parent[]` dari `destId` hingga ke *source*.

## Kompleksitas Waktu
Pencarian menggunakan BFS pada representasi adjacency list memakan waktu **$O(V + E)$**, yang mana lebih cepat ketimbang Dijkstra karena tidak memerlukan proses pengurutan dengan *Priority Queue*.