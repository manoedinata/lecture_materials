package graph;

import graph.Graph;
import model.Route;
import model.RouteResult;
import tree.MinHeap;

import java.util.ArrayList;
import java.util.List;

/**
 * Dijkstra's Algorithm — Single Source Shortest Path.
 *
 * Finds the minimum-weight path from a source node to all other nodes.
 * Supports two modes by selecting the weight to optimize:
 *   MODE_TIME : minimize total travel time (minutes)
 *   MODE_COST : minimize total cost (rupiah)
 *
 * Complexity: O((V + E) log V) using Min-Heap.
 */
public class Dijkstra {

    public static final int MODE_TIME = 0;
    public static final int MODE_COST = 1;

    private Graph graph;

    public Dijkstra(Graph graph) {
        this.graph = graph;
    }

    /**
     * Run Dijkstra from source to destination using the given mode.
     *
     * @param sourceId  starting station id
     * @param destId    target station id
     * @param mode      MODE_TIME or MODE_COST
     * @return RouteResult with path and metrics
     */
    public RouteResult findShortestPath(int sourceId, int destId, int mode) {
        int n = graph.getMaxNodes();

        int[]     dist   = new int[n];
        int[]     parent = new int[n];
        boolean[] visited= new boolean[n];

        // Initialize distances to "infinity"
        for (int i = 0; i < n; i++) {
            dist[i]    = Integer.MAX_VALUE;
            parent[i]  = -1;
            visited[i] = false;
        }

        dist[sourceId] = 0;

        MinHeap heap = new MinHeap(n);
        heap.insert(sourceId, 0);

        while (!heap.isEmpty()) {
            MinHeap.HeapNode current = heap.extractMin();
            int u = current.nodeId;

            if (visited[u]) continue;
            visited[u] = true;

            if (u == destId) break; // found destination

            // Relax all neighbors
            for (Route r : graph.getNeighbors(u)) {
                int v      = r.getToId();
                int weight = (mode == MODE_TIME) ? r.getTimeMinutes() : r.getCostRupiah();

                if (dist[u] == Integer.MAX_VALUE) continue;

                long newDist = (long) dist[u] + weight;
                if (newDist < dist[v]) {
                    dist[v]   = (int) newDist;
                    parent[v] = u;
                    if (heap.contains(v)) {
                        heap.decreaseKey(v, dist[v]);
                    } else {
                        heap.insert(v, dist[v]);
                    }
                }
            }
        }

        return buildResult(sourceId, destId, dist, parent, mode);
    }

    private RouteResult buildResult(int src, int dest, int[] dist, int[] parent, int mode) {
        RouteResult result = new RouteResult();

        if (dist[dest] == Integer.MAX_VALUE) {
            result.setFound(false);
            return result;
        }

        // Reconstruct path by tracing parent array backwards
        List<Integer> reversePath = new ArrayList<>();
        int cur = dest;
        while (cur != -1) {
            reversePath.add(cur);
            cur = parent[cur];
            if (reversePath.size() > graph.getMaxNodes()) break; // cycle guard
        }

        // Reverse to get source → destination order
        List<Integer> path = new ArrayList<>();
        for (int i = reversePath.size() - 1; i >= 0; i--) {
            path.add(reversePath.get(i));
        }

        // Calculate total time AND total cost along the path
        int totalTime = 0, totalCost = 0;
        for (int i = 0; i < path.size() - 1; i++) {
            int from = path.get(i);
            int to   = path.get(i + 1);
            for (Route r : graph.getNeighbors(from)) {
                if (r.getToId() == to) {
                    totalTime += r.getTimeMinutes();
                    totalCost += r.getCostRupiah();
                    break;
                }
            }
        }

        result.setFound(true);
        result.setPath(path);
        result.setTotalTime(totalTime);
        result.setTotalCost(totalCost);
        result.setNumTransits(path.size() - 2 > 0 ? path.size() - 2 : 0);

        return result;
    }
}
