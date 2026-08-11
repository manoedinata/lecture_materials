package graph;

import java.util.ArrayList;
import java.util.List;

import model.Route;
import model.Station;

/**
 * Directed weighted graph using adjacency list.
 * Implemented from scratch using OOP — no java.util.HashMap or library graph.
 *
 * Internally uses two parallel arrays:
 * stations[] — stores Station nodes indexed by id
 * adjList[] — stores list of Route edges for each node
 */
public class Graph {

    private final int maxNodes;

    private Station[] stations;
    private List<Route>[] adjList;
    private int numStations;
    private int numRoutes;

    public Graph() {
        this(100); // Default capacity for backward compatibility
    }

    @SuppressWarnings("unchecked")
    public Graph(int capacity) {
        this.maxNodes = capacity;
        stations = new Station[maxNodes];
        adjList = new ArrayList[maxNodes];
        numStations = 0;
        numRoutes = 0;
        for (int i = 0; i < maxNodes; i++) {
            adjList[i] = new ArrayList<>();
        }
    }

    public boolean addStation(Station s) {
        if (s.getId() < 0 || s.getId() >= maxNodes)
            return false;
        if (stations[s.getId()] != null)
            return false; // duplicate
        stations[s.getId()] = s;
        numStations++;
        return true;
    }

    public Station getStation(int id) {
        if (id < 0 || id >= maxNodes)
            return null;
        return stations[id];
    }

    public boolean removeStation(int id) {
        if (id < 0 || id >= maxNodes || stations[id] == null)
            return false;
        stations[id] = null;
        adjList[id].clear();
        // Also remove all edges pointing to this station
        for (int i = 0; i < maxNodes; i++) {
            if (stations[i] != null) {
                adjList[i].removeIf(r -> r.getToId() == id);
            }
        }
        numStations--;
        return true;
    }

    public boolean updateStation(int id, String name, String city, String type) {
        if (id < 0 || id >= maxNodes || stations[id] == null)
            return false;
        if (name != null && !name.isEmpty())
            stations[id].setName(name);
        if (city != null && !city.isEmpty())
            stations[id].setCity(city);
        if (type != null && !type.isEmpty())
            stations[id].setType(type);
        return true;
    }

    public boolean addRoute(Route r) {
        int from = r.getFromId();
        int to = r.getToId();
        if (from < 0 || from >= maxNodes || stations[from] == null)
            return false;
        if (to < 0 || to >= maxNodes || stations[to] == null)
            return false;
        adjList[from].add(r);
        numRoutes++;
        return true;
    }

    public boolean removeRoute(int fromId, int toId) {
        if (fromId < 0 || fromId >= maxNodes || stations[fromId] == null)
            return false;
        int before = adjList[fromId].size();
        adjList[fromId].removeIf(r -> r.getToId() == toId);
        int after = adjList[fromId].size();
        if (before > after) {
            numRoutes -= (before - after);
            return true;
        }
        return false;
    }

    public boolean updateRoute(int fromId, int toId, int newTime, int newCost) {
        if (fromId < 0 || fromId >= maxNodes || stations[fromId] == null)
            return false;
        for (Route r : adjList[fromId]) {
            if (r.getToId() == toId) {
                if (newTime > 0)
                    r.setTimeMinutes(newTime);
                if (newCost > 0)
                    r.setCostRupiah(newCost);
                return true;
            }
        }
        return false;
    }

    public List<Route> getNeighbors(int id) {
        if (id < 0 || id >= maxNodes || stations[id] == null)
            return new ArrayList<>();
        return adjList[id];
    }

    public void printAllStations() {
        System.out.println("\n══════════════════════════════════════════════════════════════════");
        System.out.printf("  %-4s %-25s %-20s %-6s%n", "ID", "Nama Stasiun", "Kota", "Tipe");
        System.out.println("──────────────────────────────────────────────────────────────────");
        for (int i = 0; i < maxNodes; i++) {
            if (stations[i] != null) {
                System.out.printf("  %-4d %-25s %-20s %-6s%n",
                        stations[i].getId(), stations[i].getName(),
                        stations[i].getCity(), stations[i].getType());
            }
        }
        System.out.println("══════════════════════════════════════════════════════════════════");
        System.out.printf("  Total: %d stasiun%n%n", numStations);
    }

    public void printGraph() {
        System.out.println("\n══════════════════ STRUKTUR GRAPH ══════════════════");
        for (int i = 0; i < maxNodes; i++) {
            if (stations[i] != null && !adjList[i].isEmpty()) {
                System.out.printf("  [%2d] %-25s →%n", i, stations[i].getName());
                for (Route r : adjList[i]) {
                    Station dest = stations[r.getToId()];
                    System.out.printf("       ├── %-25s | %3d mnt | Rp%,7d | %s%n",
                            dest != null ? dest.getName() : "?",
                            r.getTimeMinutes(), r.getCostRupiah(), r.getLineName());
                }
            }
        }
        System.out.println("════════════════════════════════════════════════════");
        System.out.printf("  Total: %d edge (rute)%n%n", numRoutes);
    }

    public int getNumStations() {
        return numStations;
    }

    public int getNumRoutes() {
        return numRoutes;
    }

    public int getMaxNodes() {
        return maxNodes;
    }
}
