# Min-Heap (Priority Queue Kustom)

## Konsep Dasar
Min-Heap adalah implementasi struktur data *Binary Tree* komplit di mana nilai *parent* selalu lebih kecil atau sama dengan *child*-nya. Karena strukturnya komplit, ia efisien disimpan dalam bentuk **Array Linear**.

Pada proyek ini, `MinHeap.java` dikodekan manual (*from scratch*) sebagai pengganti `java.util.PriorityQueue` bawaan, khusus untuk mempercepat operasi algoritma Dijkstra.

## Struktur Array dan Matematika Indeks
Dalam bentuk array indeks `i` (mulai dari 0):
- Indeks Parent = `(i - 1) / 2`
- Indeks Left Child = `2 * i + 1`
- Indeks Right Child = `2 * i + 2`

## Array Ajaib: `position[]`
Ini adalah kelebihan utama implementasi ini. `position[nodeId]` menyimpan indeks di mana `nodeId` tersebut berada di dalam array Heap `data[]`. 
Tanpa array `position`, ketika algoritma Dijkstra melakukan pembaruan jarak (*relaxing* bobot menjadi lebih kecil) dan harus memanggil `decreaseKey()`, ia harus melakukan iterasi array $O(N)$ untuk mencari lokasi node.
Dengan `position`, lokasi ditemukan dalam $O(1)$, menjadikan `decreaseKey` hanya memakan logik *bubble up* senilai $O(\log N)$.

## Mekanisme Operasi Dasar

### 1. Insert
- Masukkan elemen baru ke ujung array (akhir tree).
- Lakukan **`bubbleUp`**: Bandingkan dengan parent, jika prioritas baru lebih kecil, lakukan `swap`. Ulangi hingga root atau sampai parent lebih kecil.

### 2. Extract-Min
- Node prioritas terkecil (jarak terdekat) selalu berada di `data[0]` (root).
- Simpan nilainya, lalu timpa root dengan elemen terakhir di heap.
- Hapus elemen terakhir (kurangi `size`).
- Lakukan **`bubbleDown`**: Bandingkan root baru dengan kedua anaknya. Tukar (`swap`) dengan anak yang paling kecil. Ulangi ke bawah hingga properti Min-Heap terjaga.

### 3. Decrease-Key
- Jika jarak terbaru lebih kecil dari yang terdaftar di heap, ubah nilai `priority` pada elemen tersebut.
- Karena nilainya mengecil (lebih prioritas), elemen tersebut harus naik kelas. Panggil `bubbleUp(idx)`.

## Kompleksitas
- **Insert**: $O(\log N)$
- **Extract-Min**: $O(\log N)$
- **Decrease Key**: $O(\log N)$