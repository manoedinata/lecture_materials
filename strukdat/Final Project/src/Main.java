import graph.BFS;
import graph.Dijkstra;
import graph.Graph;
import model.Route;
import model.RouteResult;
import model.Station;
import server.ApiServer;
import tree.Trie;

import java.util.List;
import java.util.Scanner;

/**
 * TransportApp — Main interactive console application.
 *
 * Menu:
 * 1. Cari halte/stasiun berdasarkan nama (Trie search)
 * 2. Cari rute tercepat (Dijkstra - waktu)
 * 3. Cari rute termurah (Dijkstra - biaya)
 * 4. Cari rute transit paling sedikit (BFS)
 * 5. Bandingkan ketiga kriteria rute (HOTS comparison)
 * 6. Simulasikan rute yang tidak tersedia (virtual route)
 * 7. Tampilkan semua stasiun
 * 8. Tampilkan struktur graph
 * 9. Insert / Update / Delete data
 * 0. Keluar
 */
public class Main {

    private Graph graph;
    private Trie trie;
    private Dijkstra dijkstra;
    private BFS bfs;
    private Scanner sc;

    public Main() {
        graph = new Graph();
        trie = new Trie();
        dijkstra = new Dijkstra(graph);
        bfs = new BFS(graph);
        sc = new Scanner(System.in);
    }

    public void run() {
        DataLoader.load(graph, trie);

        // Start Web API Server for visualizations
        try {
            ApiServer server = new ApiServer(graph);
            server.start(8080);
        } catch (Exception e) {
            System.err.println("Gagal menjalankan API Server: " + e.getMessage());
        }

        printBanner();

        boolean running = true;
        while (running) {
            printMenu();
            String choice = sc.nextLine().trim();
            switch (choice) {
                case "1":
                    menuSearchStation();
                    break;
                case "2":
                    menuFastestRoute();
                    break;
                case "3":
                    menuCheapestRoute();
                    break;
                case "4":
                    menuMinTransitRoute();
                    break;
                case "5":
                    menuCompareRoutes();
                    break;
                case "6":
                    menuSimulateRoute();
                    break;
                case "7":
                    graph.printAllStations();
                    break;
                case "8":
                    graph.printGraph();
                    break;
                case "9":
                    menuDataManagement();
                    break;
                case "0":
                    running = false;
                    break;
                default:
                    System.out.println("   Pilihan tidak valid.\n");
            }
        }
        System.out.println("\n  Terima kasih telah menggunakan Public Transport Planner! \n");
        sc.close();
    }

    private void menuSearchStation() {
        System.out.println("\n── [1] CARI STASIUN ──────────────────────────────");
        System.out.print("  Masukkan nama atau awalan stasiun: ");
        String query = sc.nextLine().trim();

        // First try exact match
        int exactId = trie.search(query);
        if (exactId != -1) {
            Station s = graph.getStation(exactId);
            System.out.println("\n   Ditemukan (exact match):");
            System.out.println("  " + s);
        } else {
            // Prefix search
            List<Integer> ids = trie.searchByPrefix(query);
            if (ids.isEmpty()) {
                System.out.println("  x Tidak ada stasiun yang cocok dengan \"" + query + "\".");
            } else {
                System.out.println("\n   Ditemukan " + ids.size() + " stasiun dengan awalan \"" + query + "\":");
                for (int id : ids) {
                    Station s = graph.getStation(id);
                    if (s != null)
                        System.out.println("    • " + s);
                }
            }
        }
        System.out.println();
    }

    private void menuFastestRoute() {
        System.out.println("\n── [2] RUTE TERCEPAT ─────────────────────────────");
        int[] ids = pickSourceDest();
        if (ids == null)
            return;

        RouteResult result = dijkstra.findShortestPath(ids[0], ids[1], Dijkstra.MODE_TIME);
        System.out.println("\n   Hasil Rute Tercepat:");
        result.print(graph);
        System.out.println();
    }

    private void menuCheapestRoute() {
        System.out.println("\n── [3] RUTE TERMURAH ─────────────────────────────");
        int[] ids = pickSourceDest();
        if (ids == null)
            return;

        RouteResult result = dijkstra.findShortestPath(ids[0], ids[1], Dijkstra.MODE_COST);
        System.out.println("\n   Hasil Rute Termurah:");
        result.print(graph);
        System.out.println();
    }

    private void menuMinTransitRoute() {
        System.out.println("\n── [4] RUTE TRANSIT PALING SEDIKIT ───────────────");
        int[] ids = pickSourceDest();
        if (ids == null)
            return;

        RouteResult result = bfs.findMinTransitPath(ids[0], ids[1]);
        System.out.println("\n   Hasil Rute Transit Minimum (BFS):");
        result.print(graph);
        System.out.println();
    }

    private void menuCompareRoutes() {
        System.out.println("\n── [5] BANDINGKAN TIGA KRITERIA RUTE ─────────────");
        int[] ids = pickSourceDest();
        if (ids == null)
            return;

        int src = ids[0];
        int dest = ids[1];

        RouteResult fastest = dijkstra.findShortestPath(src, dest, Dijkstra.MODE_TIME);
        RouteResult cheapest = dijkstra.findShortestPath(src, dest, Dijkstra.MODE_COST);
        RouteResult minTrans = bfs.findMinTransitPath(src, dest);

        Station s = graph.getStation(src);
        Station d = graph.getStation(dest);
        System.out.println("\n  ╔══════════════════════════════════════════════════════════╗");
        System.out.printf("  ║  PERBANDINGAN RUTE: %-38s║%n",
                (s != null ? s.getName() : src) + " → " + (d != null ? d.getName() : dest));
        System.out.println("  ╠══════════════════════════════════════════════════════════╣");

        printCompareRow(" Rute Tercepat", fastest, graph);
        printCompareRow(" Rute Termurah", cheapest, graph);
        printCompareRow(" Transit Minimum", minTrans, graph);

        System.out.println("  ╠══════════════════════════════════════════════════════════╣");
        System.out.println("  ║   ANALISIS HOTS                                        ║");
        System.out.println("  ╠══════════════════════════════════════════════════════════╣");

        printHotsAnalysis(fastest, cheapest, minTrans);

        System.out.println("  ╚══════════════════════════════════════════════════════════╝\n");
    }

    private void printCompareRow(String label, RouteResult r, Graph g) {
        if (!r.isFound()) {
            System.out.printf("  ║  %-20s : Tidak ditemukan%-24s║%n", label, "");
            return;
        }
        System.out.printf("  ║  %-20s : %3d mnt | Rp%,7d | %d transit%-6s║%n",
                label, r.getTotalTime(), r.getTotalCost(), r.getNumTransits(), "");
    }

    private void printHotsAnalysis(RouteResult fast, RouteResult cheap, RouteResult transit) {
        boolean sameFastCheap = fast.isFound() && cheap.isFound() && fast.getPath().equals(cheap.getPath());
        boolean sameFastTransit = fast.isFound() && transit.isFound() && fast.getPath().equals(transit.getPath());
        boolean allSame = sameFastCheap && sameFastTransit;

        if (allSame) {
            System.out.println("  ║   Ketiga kriteria menghasilkan rute yang SAMA.        ║");
            System.out.println("  ║     Ini terjadi karena rute langsung mendominasi        ║");
            System.out.println("  ║     semua dimensi (waktu, biaya, dan transit).          ║");
        } else {
            System.out.println("  ║   Ketiga kriteria menghasilkan rute BERBEDA karena:   ║");
            if (!sameFastCheap) {
                System.out.println("  ║  • Rute cepat ≠ murah: rute ekspres menghemat waktu    ║");
                System.out.println("  ║    tapi lebih mahal daripada rute dengan transit.       ║");
            }
            if (!sameFastTransit) {
                System.out.println("  ║  • Rute cepat ≠ transit minimum: jalur langsung        ║");
                System.out.println("  ║    bisa lebih lambat dari transfer di stasiun hub.      ║");
            }
            System.out.println("  ║  =>> Ini adalah trade-off dalam multi-objective graph.    ║");
        }
    }

    private void menuSimulateRoute() {
        System.out.println("\n── [6] SIMULASI RUTE TIDAK TERSEDIA ──────────────");
        System.out.println("  Fitur ini menambahkan rute sementara antara dua stasiun");
        System.out.println("  yang belum terhubung, lalu mencari rute terbaik.\n");

        int[] ids = pickSourceDest();
        if (ids == null)
            return;
        int src = ids[0];
        int dest = ids[1];

        // Check if direct route already exists
        boolean directExists = false;
        for (Route r : graph.getNeighbors(src)) {
            if (r.getToId() == dest) {
                directExists = true;
                break;
            }
        }

        if (directExists) {
            System.out.println(" Rute langsung sudah ada antara kedua stasiun tersebut.");
        } else {
            System.out.print("  Masukkan waktu tempuh simulasi (menit): ");
            int simTime = readInt(1, 300);
            System.out.print("  Masukkan biaya simulasi (Rp): ");
            int simCost = readInt(0, 500000);

            // Add temporary route
            Route simRoute = new Route(src, dest, simTime, simCost,
                    "*** RUTE SIMULASI ***", "SIM");
            graph.addRoute(simRoute);

            System.out.println("\n   Rute simulasi ditambahkan sementara.");
            System.out.println("  Mencari rute dengan simulasi...\n");

            RouteResult fastest = dijkstra.findShortestPath(src, dest, Dijkstra.MODE_TIME);
            RouteResult cheapest = dijkstra.findShortestPath(src, dest, Dijkstra.MODE_COST);
            RouteResult minTrans = bfs.findMinTransitPath(src, dest);

            System.out.println("  ⏱ Rute Tercepat (dengan simulasi):");
            fastest.print(graph);
            System.out.println("   Rute Termurah (dengan simulasi):");
            cheapest.print(graph);
            System.out.println("   Transit Minimum (dengan simulasi):");
            minTrans.print(graph);

            // Remove temporary route
            graph.removeRoute(src, dest);
            System.out.println("\n  ℹ Rute simulasi telah dihapus dari sistem.\n");
        }
    }

    private void menuDataManagement() {
        System.out.println("\n── [9] MANAJEMEN DATA ─────────────────────────────");
        System.out.println("  a. Tambah stasiun baru");
        System.out.println("  b. Tambah rute baru");
        System.out.println("  c. Update stasiun");
        System.out.println("  d. Update rute");
        System.out.println("  e. Hapus stasiun");
        System.out.println("  f. Hapus rute");
        System.out.print("  Pilihan: ");
        String sub = sc.nextLine().trim().toLowerCase();

        switch (sub) {
            case "a":
                insertStation();
                break;
            case "b":
                insertRoute();
                break;
            case "c":
                updateStation();
                break;
            case "d":
                updateRoute();
                break;
            case "e":
                deleteStation();
                break;
            case "f":
                deleteRoute();
                break;
            default:
                System.out.println("   Pilihan tidak valid.");
        }
        System.out.println();
    }

    private void insertStation() {
        System.out.println("\n  === Tambah Stasiun Baru ===");
        System.out.print("  ID baru (angka): ");
        int id = readInt(0, 99);
        if (graph.getStation(id) != null) {
            System.out.println("   ID sudah digunakan.");
            return;
        }
        System.out.print("  Nama stasiun   : ");
        String name = sc.nextLine().trim();
        System.out.print("  Kota           : ");
        String city = sc.nextLine().trim();
        System.out.print("  Tipe (KRL/MRT/BUS/KA/LRT/TRANS): ");
        String type = sc.nextLine().trim().toUpperCase();

        Station s = new Station(id, name, city, type, 0.0, 0.0);
        graph.addStation(s);
        trie.insert(name, id);
        System.out.println("   Stasiun \"" + name + "\" berhasil ditambahkan (ID=" + id + ").");
    }

    private void insertRoute() {
        System.out.println("\n  === Tambah Rute Baru ===");
        graph.printAllStations();
        System.out.print("  ID stasiun asal : ");
        int from = readInt(0, 99);
        System.out.print("  ID stasiun tujuan: ");
        int to = readInt(0, 99);
        if (graph.getStation(from) == null || graph.getStation(to) == null) {
            System.out.println("   Stasiun tidak ditemukan.");
            return;
        }
        System.out.print("  Waktu (menit)   : ");
        int time = readInt(1, 600);
        System.out.print("  Biaya (Rp)      : ");
        int cost = readInt(0, 1000000);
        System.out.print("  Nama jalur      : ");
        String line = sc.nextLine().trim();

        Route r = new Route(from, to, time, cost, line, "CUSTOM");
        graph.addRoute(r);
        System.out.printf("   Rute %s → %s berhasil ditambahkan.%n",
                graph.getStation(from).getName(), graph.getStation(to).getName());
    }

    private void updateStation() {
        System.out.println("\n  === Update Stasiun ===");
        System.out.print("  ID stasiun yang diupdate: ");
        int id = readInt(0, 99);
        if (graph.getStation(id) == null) {
            System.out.println("   Stasiun tidak ditemukan.");
            return;
        }
        System.out.println("  (Biarkan kosong jika tidak diubah)");
        System.out.print("  Nama baru: ");
        String name = sc.nextLine().trim();
        System.out.print("  Kota baru: ");
        String city = sc.nextLine().trim();
        System.out.print("  Tipe baru: ");
        String type = sc.nextLine().trim().toUpperCase();
        graph.updateStation(id, name, city, type);
        System.out.println("   Stasiun ID=" + id + " berhasil diupdate.");
    }

    private void updateRoute() {
        System.out.println("\n  === Update Rute ===");
        System.out.print("  ID stasiun asal : ");
        int from = readInt(0, 99);
        System.out.print("  ID stasiun tujuan: ");
        int to = readInt(0, 99);
        System.out.print("  Waktu baru (menit, 0=tidak berubah): ");
        int time = readInt(0, 600);
        System.out.print("  Biaya baru (Rp,   0=tidak berubah): ");
        int cost = readInt(0, 1000000);
        if (graph.updateRoute(from, to, time, cost)) {
            System.out.println("   Rute berhasil diupdate.");
        } else {
            System.out.println("   Rute tidak ditemukan.");
        }
    }

    private void deleteStation() {
        System.out.println("\n  === Hapus Stasiun ===");
        System.out.print("  ID stasiun yang dihapus: ");
        int id = readInt(0, 99);
        Station s = graph.getStation(id);
        if (s == null) {
            System.out.println("   Stasiun tidak ditemukan.");
            return;
        }
        trie.delete(s.getName());
        graph.removeStation(id);
        System.out.println("   Stasiun \"" + s.getName() + "\" berhasil dihapus.");
    }

    private void deleteRoute() {
        System.out.println("\n  === Hapus Rute ===");
        System.out.print("  ID stasiun asal : ");
        int from = readInt(0, 99);
        System.out.print("  ID stasiun tujuan: ");
        int to = readInt(0, 99);
        if (graph.removeRoute(from, to)) {
            System.out.println("   Rute berhasil dihapus.");
        } else {
            System.out.println("   Rute tidak ditemukan.");
        }
    }

    /**
     * Ask user to pick source and destination stations.
     * Supports both ID input and name search via Trie.
     */
    private int[] pickSourceDest() {
        System.out.println("  (Masukkan ID angka atau nama stasiun)");
        System.out.print("  Stasiun ASAL   : ");
        int src = resolveStation(sc.nextLine().trim());
        if (src == -1) {
            System.out.println("   Stasiun asal tidak ditemukan.\n");
            return null;
        }

        System.out.print("  Stasiun TUJUAN : ");
        int dest = resolveStation(sc.nextLine().trim());
        if (dest == -1) {
            System.out.println("   Stasiun tujuan tidak ditemukan.\n");
            return null;
        }

        if (src == dest) {
            System.out.println("   Asal dan tujuan sama.\n");
            return null;
        }

        Station s = graph.getStation(src);
        Station d = graph.getStation(dest);
        System.out.printf("  ► %s → %s%n", s.getName(), d.getName());
        return new int[] { src, dest };
    }

    private int resolveStation(String input) {
        // Try parsing as integer ID
        try {
            int id = Integer.parseInt(input);
            return graph.getStation(id) != null ? id : -1;
        } catch (NumberFormatException e) {
            // Try exact name search via Trie
            int id = trie.search(input);
            if (id != -1)
                return id;
            // Try prefix search
            List<Integer> results = trie.searchByPrefix(input);
            if (results.size() == 1)
                return results.get(0);
            if (results.size() > 1) {
                System.out.println("  Beberapa stasiun cocok:");
                for (int rid : results) {
                    Station s = graph.getStation(rid);
                    System.out.printf("    [%d] %s%n", rid, s != null ? s.getName() : "?");
                }
                System.out.print("  Pilih ID: ");
                try {
                    int chosen = Integer.parseInt(sc.nextLine().trim());
                    return graph.getStation(chosen) != null ? chosen : -1;
                } catch (NumberFormatException ex) {
                    return -1;
                }
            }
            return -1;
        }
    }

    private int readInt(int min, int max) {
        while (true) {
            try {
                String line = sc.nextLine().trim();
                int val = Integer.parseInt(line);
                if (val >= min && val <= max)
                    return val;
                System.out.printf("   Masukkan angka antara %d dan %d: ", min, max);
            } catch (NumberFormatException e) {
                System.out.print("   Input tidak valid, coba lagi: ");
            }
        }
    }

    private void printBanner() {
        System.out.println();
        System.out.println("  ╔══════════════════════════════════════════════════╗");
        System.out.println("  ║         PUBLIC TRANSPORT PLANNER                 ║");
        System.out.println("  ╠══════════════════════════════════════════════════╣");
        System.out.printf("  ║  Stasiun: %-5d | Rute: %-5d                    ║%n",
                graph.getNumStations(), graph.getNumRoutes());
        System.out.println("  ║  Struktur : Graph (Adjacency List) + MinHeap     ║");
        System.out.println("  ║           : Trie untuk pencarian nama            ║");
        System.out.println("  ║  Algoritma: Dijkstra (waktu/biaya) + BFS         ║");
        System.out.println("  ╚══════════════════════════════════════════════════╝");
        System.out.println();
    }

    private void printMenu() {
        System.out.println("  ┌─────────────────────────────────────────────────┐");
        System.out.println("  │                    MENU UTAMA                   │");
        System.out.println("  ├─────────────────────────────────────────────────┤");
        System.out.println("  │  1. Cari stasiun berdasarkan nama               │");
        System.out.println("  │  2. Cari rute tercepat          (Dijkstra-Waktu)│");
        System.out.println("  │  3. Cari rute termurah          (Dijkstra-Biaya)│");
        System.out.println("  │  4. Cari rute transit minimum   (BFS)           │");
        System.out.println("  │  5. Bandingkan 3 kriteria rute                  │");
        System.out.println("  │  6. Simulasi rute tidak tersedia                │");
        System.out.println("  │  7. Tampilkan semua stasiun                     │");
        System.out.println("  │  8. Tampilkan struktur graph                    │");
        System.out.println("  │  9. Manajemen data (Insert/Update/Delete)       │");
        System.out.println("  │  0. Keluar                                      │");
        System.out.println("  └─────────────────────────────────────────────────┘");
        System.out.print("  Pilihan: ");
    }

    public static void main(String[] args) {
        new Main().run();
    }
}
