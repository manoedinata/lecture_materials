# Public Transport Planner

## Kelompok 2

1. Hasheemi Rafsanjani - 5027251015
2. Hendra Manudinata - 5027251051
3. Nabila Sharliz Sigit - 5027251054
4. Rido Patra Yudhistira Edwin - 5027251120
5. Rayhan Fadhilah Allayn - 5027151126

## Penjelasan

Aplikasi **Public Transport Planner** yang kami rancang adalah sistem pencarian rute transportasi umum tercepat yang diimplementasikan menggunakan dua struktur data utama: **Tree (Trie)** untuk fitur _search_ dan _autocomplete_ nama stasiun/halte dengan kompleksitas O(N), serta **Graph** yang merepresentasikan keterhubungan rute antar stasiun sebagai _Node (Vertex)_ dan _Edge_ berbobot (waktu/biaya), dengan pencarian rute tercepat dieksekusi menggunakan algoritma **Dijkstra** atau **BFS**.

## Tree Directory

Struktur direktori utama aplikasi adalah sebagai berikut:

```text
.
├── src/
│   ├── DataLoader.java
│   ├── Main.java
│   ├── graph/       # BFS.java, Dijkstra.java, Graph.java, SimpleQueue.java
│   ├── model/       # Route.java, RouteResult.java, Station.java
│   └── tree/        # MinHeap.java, Trie.java
├── data/
│   ├── routes.csv
│   ├── routes_jatim.csv
│   ├── stations.csv
│   └── stations_jatim.csv
├── docs/
│   └── laporan.pdf
├── compile_and_run.bat
└── compile_and_run.sh
```

## Cara Compile

Anda dapat langsung menggunakan _script_ yang sudah disediakan untuk melakukan _compile_ dan _run_ aplikasi.

**Untuk Windows:**
Buka Command Prompt atau PowerShell dan jalankan:

```bat
.\compile_and_run.bat
```

**Untuk Linux / macOS:**
Buka terminal dan jalankan:

```bash
./compile_and_run.sh
```

_(Pastikan Anda sudah menginstal JDK dan env `javac` & `java` sudah terdaftar di sistem)._
