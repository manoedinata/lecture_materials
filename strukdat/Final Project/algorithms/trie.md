# Trie (Prefix Tree)

## Konsep Dasar
Trie adalah struktur data pohon (tree) yang digunakan untuk menyimpan koleksi string (biasanya kamus/kumpulan kata) sehingga pencarian string dapat dilakukan dengan sangat cepat. Pada proyek ini, Trie digunakan untuk fitur pencarian nama stasiun dan **autocomplete**.

## Struktur Node (`TrieNode`)
Setiap node pada Trie di proyek ini memiliki:
- `children[]`: Array berukuran 128 (mewakili karakter ASCII). Ini menghindari penggunaan `HashMap` bawaan sehingga lebih murni dari segi struktur data dasar.
- `isEnd`: Boolean penanda apakah node tersebut merupakan akhir dari sebuah nama stasiun yang valid.
- `stationId`: Integer yang menyimpan ID stasiun jika `isEnd` bernilai `true`.
- `ch`: Karakter node (digunakan terutama untuk proses debung/tracing).

## Mekanisme Operasi Utama

### 1. Insert (Penyisipan Kata)
Ketika sistem membaca nama stasiun (misal: "Juanda"), string tersebut diubah menjadi huruf kecil (*lowercase*). Algoritma akan melakukan iterasi huruf demi huruf:
- Mengecek indeks array ASCII dari karakter tersebut.
- Jika `children[karakter]` masih `null`, buat `TrieNode` baru.
- Bergerak ke node *child* tersebut.
- Pada huruf terakhir ('a'), tandai `isEnd = true` dan simpan `stationId`.

### 2. Exact Search (Pencarian Tepat)
Mengikuti lintasan node huruf demi huruf dari *query*. Jika karakter putus di tengah jalan, atau node terakhir memiliki `isEnd == false`, maka kata tidak ditemukan (mengembalikan `-1`). Jika ditemukan, langsung kembalikan `stationId`.
**Kompleksitas**: $O(L)$ dimana $L$ adalah panjang string.

### 3. Prefix Search / Autocomplete (`searchByPrefix`)
Mekanisme ini sangat berguna untuk pencarian sebagian kata (contoh: *query* "Man" untuk mencari "Manggarai").
1. **Traversal Awal**: Ikuti karakter dari prefix ("m" -> "a" -> "n") sampai menemukan node terakhir dari prefix tersebut.
2. **DFS (Depth-First Search)**: Mulai dari node 'n' tersebut, lakukan rekursi DFS ke seluruh *children* yang tidak `null` (`collectAll`).
3. Setiap menemukan node dengan `isEnd == true`, masukkan `stationId`-nya ke dalam *list* hasil (koleksi kandidat *autocomplete*).

## Kelebihan dalam Proyek
Pencarian string dengan *Trie* jauh lebih efisien $O(L)$ dibandingkan dengan iterasi manual menggunakan `.contains()` atau `.startsWith()` pada kumpulan *array/list* stasiun yang akan memakan waktu $O(N \times L)$ di mana $N$ adalah jumlah seluruh stasiun.