# Tracing

Tracing manual untuk mempermudah memahami cara kerja algoritma tree, graph, dan menelusuri ketika berpapasan dengan edge case

## 1. Tracing Tree - Insert

Tracing insert "Manggarai", dimulai dengan lowercase string kemudian displit per character:

| Iterasi | Proses                             | Keterangan                                                                                                                         |
| ------- | ---------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------- |
| 1       | [i=0] Baca 'm'                     | curr = root, children[109] == null → buat TrieNode('m'), curr = TrieNode('m')                                                      |
| 2       | [i=1] Baca 'a'                     | curr = TrieNode('m'), children[97] == null → buat TrieNode('a'), curr = TrieNode('a')                                              |
| 3       | [i=2] Baca 'n'                     | curr = TrieNode('a'), children[110] == null → buat TrieNode('n'), curr = TrieNode('n')                                             |
| 4       | [i=3] Baca 'g'                     | curr = TrieNode('n'), children[103] == null → buat TrieNode('g'), curr = TrieNode('g')                                             |
| 5       | [i=4] Baca 'g'                     | curr = TrieNode('g'), children[103] == null → buat TrieNode('g'), curr = TrieNode('g')                                             |
| 6       | [i=5] Baca 'a'                     | curr = TrieNode('g'), children[97] == null → buat TrieNode('a'), curr = TrieNode('a')                                              |
| 7       | [i=6] Baca 'r'                     | curr = TrieNode('a'), children[114] == null → buat TrieNode('r'), curr = TrieNode('r')                                             |
| 8       | [i=7] Baca 'a'                     | curr = TrieNode('r'), children[97] == null → buat TrieNode('a'), curr = TrieNode('a') [depth=8]                                    |
| 9       | [i=8] Baca 'i' — karakter terakhir | curr = TrieNode('a')[depth=8], children[105] == null → buat TrieNode('i'), curr = TrieNode('i'), Loop selesai (depth = length = 9) |
| 10      | [DONE] Set terminal node           | curr.isEnd = true (sebelumnya false), curr.stationId = 1, count++ → count = 1, Insert selesai: 9 node baru dibuat di Trie          |

**Keterangan:** Setelah muncul stationId maka insert telah sukses dilakukan dan string nama stasiun/halte sudah masuk ke data dan dapat dilakukan search.

## 2. Tracing Tree - Search

Tracing search dengan string "Mang" untuk menghasilkan autocomplete "Manggarai":

| Langkah | Proses                             | Keterangan                                                                                                                                                         |
| ------- | ---------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| 1       | Lowercase input                    | query = 'mang'.toLowerCase() = 'mang'                                                                                                                              |
| 2       | findNode('mang') — traversal       | i=0: curr = root → root.children['m'] ada → curr = TrieNode('m')                                                                                                   |
|         |                                    | i=1: curr.children['a'] ada → curr = TrieNode('a')                                                                                                                 |
|         |                                    | i=2: curr.children['n'] ada → curr = TrieNode('n')                                                                                                                 |
|         |                                    | i=3: curr.children['g'] ada → curr = TrieNode('g')                                                                                                                 |
|         |                                    | return TrieNode('g') — prefix node ditemukan                                                                                                                       |
| 3       | collectAll(TrieNode('g'), results) | DFS dari node 'g': g → g → a → r → a → i [isEnd=true] Tambahkan stationId=1 ke results list, return results = [1]                                                  |
| 4       | Output akhir                       | searchByPrefix('Mang') → [1], graph.getStation(1) → Station[id=1, name='Manggarai', city='Jakarta Selatan', type='KRL'] ✓ Ditemukan 1 stasiun dengan prefix 'Mang' |

**Kompleksitas:** O(N) dengan N adalah panjang string inputan searchByPrefix.

## 3. Tracing Graph - Dijkstra

Simulasi pencarian rute tercepat dari Gambir (node 0) ke Bogor (node 8):

**Data Relasi Stasiun:**

| Dari             | Ke               | Waktu (menit) | Biaya (Rp) | Jalur                   |
| ---------------- | ---------------- | ------------- | ---------- | ----------------------- |
| 0 (Gambir)       | 13 (Juanda)      | 5             | 2.000      | Feeder Gambir-Juanda    |
| 0 (Gambir)       | 8 (Bogor)        | 70            | 35.000     | KA Pangrango (langsung) |
| 13 (Juanda)      | 14 (Sawah Besar) | 3             | 3.000      | KRL Bogor Line          |
| 14 (Sawah Besar) | 2 (Jakarta Kota) | 5             | 3.000      | KRL Bogor Line          |
| 1 (Manggarai)    | 6 (Pasar Minggu) | 14            | 4.000      | KRL Bogor Line          |
| 6 (Pasar Minggu) | 7 (Depok)        | 18            | 4.000      | KRL Bogor Line          |
| 7 (Depok)        | 8 (Bogor)        | 30            | 4.000      | KRL Bogor Line          |

**State Awal Dijkstra:**

| Variabel  | Nilai Awal                                   |
| --------- | -------------------------------------------- |
| dist[]    | dist[0]=0, dist[semua lain]=MAX_VALUE (∞)    |
| parent[]  | parent[semua]=-1 (belum ada parent)          |
| visited[] | semua = false                                |
| MinHeap   | insert(nodeId=0, priority=0) → Heap: [(0:0)] |

**Proses Iterasi Dijkstra:**

**Iterasi 1:**

- extractMin() → u=0 (Gambir), dist=0
- visited[0] = true
- Proses tetangga node 0:
  - node 13 (Juanda): dist[0]+5=5 < ∞ → UPDATE dist[13]=5, parent[13]=0
  - node 8 (Bogor): dist[0]+70=70 < ∞ → UPDATE dist[8]=70, parent[8]=0
- Heap setelah: [(13:5), (8:70)]

**Iterasi 2:**

- extractMin() → u=13 (Juanda), dist=5
- visited[13] = true
- Proses tetangga node 13:
  - node 14 (Sawah Besar): dist[13]+3=8 < ∞ → UPDATE dist[14]=8, parent[14]=13
  - node 1 (Manggarai): dist[13]+5=10 < ∞ → UPDATE dist[1]=10, parent[1]=13
  - node 0 (Gambir): visited
- Heap setelah: [(1:10), (14:8), (8:70)]

**Iterasi 3:**

- extractMin() → u=14 (Sawah Besar), dist=8
- visited[14] = true
- Proses tetangga node 14:
  - node 2 (Jakarta Kota): dist[14]+5=13 < ∞ → UPDATE dist[2]=13, parent[2]=14
  - node 1 (Manggarai): dist[14]+? = 8+? (tidak update karena rute melalui 13 lebih pendek)
  - node 13 (Juanda): visited
- Heap setelah: [(1:10), (2:13), (8:70)]

**Iterasi 4:**

- extractMin() → u=1 (Manggarai), dist=10
- visited[1] = true
- Proses tetangga node 1:
  - node 6 (Pasar Minggu): dist[1]+14=24 < ∞ → UPDATE dist[6]=24, parent[6]=1
- Heap setelah: [(2:13), (6:24), (8:70)]

**Iterasi 5:**

- extractMin() → u=2 (Jakarta Kota), dist=13
- visited[2] = true
- (Tetangga node 2 tidak relevan untuk rute ke Bogor)
- Heap setelah: [(6:24), (8:70)]

**Iterasi 6:**

- extractMin() → u=6 (Pasar Minggu), dist=24
- visited[6] = true
- Proses tetangga node 6:
  - node 7 (Depok): dist[6]+18=42 < ∞ → UPDATE dist[7]=42, parent[7]=6
- Heap setelah: [(7:42), (8:70)]

**Iterasi 7:**

- extractMin() → u=7 (Depok), dist=42
- visited[7] = true
- Proses tetangga node 7:
  - node 8 (Bogor): dist[7]+30=72 vs dist[8]=70 → 72 > 70, TIDAK UPDATE dist[8] tetap 70 (via rute langsung Gambir→Bogor KA Pangrango)
- Heap setelah: [(8:70)]

**Iterasi 8:**

- extractMin() → u=8 (Bogor), dist=70 ← DESTINATION!
- visited[8] = true
- u == destId (8) → BREAK — algoritma berhenti
- dist[8] = 70 menit (rute optimal ditemukan)

**Hasil Akhir:** Rute optimal Gambir → Bogor dengan total waktu tempuh 70 menit (langsung).

## 4. Tracing Edge Case - Same Station

**Input:** User memasukkan stasiun asal = Gambir (node 0) dan stasiun tujuan = Gambir (node 0)

| Langkah | Proses                                          | Keterangan                                                                                         |
| ------- | ----------------------------------------------- | -------------------------------------------------------------------------------------------------- |
| 1       | User memasukkan source=0, dest=0                | resolveStation('Gambir') → 0, resolveStation('Gambir') → 0 (lagi)                                  |
| 2       | [Guard] pickSourceDest() mendeteksi src == dest | if (src == dest) → System.out.println('x Asal dan tujuan sama.')                                   |
| 3       | return null                                     | Tidak lanjut ke algoritma, Dijkstra/BFS TIDAK pernah dipanggil                                     |
| 4       | [Safe] Sistem aman                              | Tidak ada infinite loop atau hasil salah, Penanganan dilakukan di layer UI sebelum menyentuh Graph |

**Output:** 'x Asal dan tujuan sama.'

## 5. Tracing Edge Case - Node Not Connected

**Input:** User memasukkan stasiun asal = Gambir (node 0) dan stasiun tujuan = Stasiun Tak Terhubung (node 99)

| Langkah | Proses                                              | Keterangan                                                                                                                                                                                                        |
| ------- | --------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 1       | [Init] Inisialisasi BFS                             | visited[0..99] = false, parent[0..99] = -1, queue = SimpleQueue, enqueue(0), visited[0] = true                                                                                                                    |
| 2       | [Pop 0] dequeue() → u=0 (Gambir)                    | u=0 ≠ destId=99, lanjut, Tetangga node 0: [13(Juanda), 8(Bogor)] → enqueue(13), visited[13]=true, parent[13]=0 → enqueue(8), visited[8]=true, parent[8]=0, Queue: [13, 8]                                         |
| 3       | [Pop 13] dequeue() → u=13                           | Tetangga 13: [14, 1, 0(visited)] → enqueue(14), enqueue(1) — keduanya belum dikunjungi, Queue: [8, 14, 1]                                                                                                         |
| 4       | [...] Proses berlanjut ke semua node yang terhubung | Node 8, 14, 1, 6, 7, 2, 3, 4, 5, 9, 10, 11, 12, ... dst, Semua 25 node yang ada di jaringan akan dikunjungi, Node 99 TIDAK ADA di adjacency list manapun → tidak pernah di-enqueue, Queue akhirnya menjadi kosong |
| 5       | [EMPTY] queue.isEmpty() = true — LOOP BERAKHIR      | found = false (belum pernah mencapai u==destId==99), BFS selesai tanpa menemukan destination                                                                                                                      |

**Output:** "X Tidak ada rute yang ditemukan."

---
