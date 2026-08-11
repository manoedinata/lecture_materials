# Struktur Graf (Graph & Adjacency List)

## Konsep Dasar
Graf dalam aplikasi transport ini memetakan Stasiun sebagai Vertex/Node, dan Jalur Kereta/Bus sebagai Edge berbobot. Implementasinya menggunakan konsep **Adjacency List**.

## Implementasi
Struktur utama di kelas `Graph.java`:
- `Station[] stations`: Array (sebagai simulasi Hash Map) di mana indeks array secara langsung mewakili *ID Stasiun*.
- `List<Route>[] adjList`: Array of List. Indeks array mewakili stasiun asal, dan daftar list-nya menampung *object* rute (Edge) ke tetangga-tetangganya.

## Mengapa Adjacency List?
Graf jaringan transportasi umum biasanya berbentuk **Sparse Graph** (Graf renggang). Stasiun Manggarai mungkin punya 5 percabangan rute, tapi tidak akan terhubung secara langsung ke 99 stasiun lain. 
Jika menggunakan matriks (*Adjacency Matrix*), sistem akan membuang memori sebesar $O(V^2)$. 
Dengan menggunakan Adjacency List, komputasi ruang memori hanya sebatas $O(V + E)$ dan iterasi untuk mengambil "tetangga rute" dalam algoritma Dijkstra atau BFS bekerja pada kecepatan yang sangat tinggi.

## Dukungan Edge Berarah (Directed Graph)
Struktur ini memfasilitasi penanganan rute searah secara modular. Meski sistem perkeretaan Indonesia pada umumnya bolak-balik (Bi-directional), DataLoader di program memanggil penambahan edge dua kali:
`graph.addRoute(a -> b)` dan `graph.addRoute(b -> a)`.
Namun jika di masa depan terdapat jalan satu arah (misalnya koridor putaran balik TransJakarta), maka sistem sudah *ready* dengan menambah rute secara satu arah di `adjList` milik stasiun asal saja.