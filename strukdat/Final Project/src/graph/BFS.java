package graph;

import graph.Graph;
import model.Route;
import model.RouteResult;

import java.util.ArrayList;
import java.util.List;

/**
 * BFS (Breadth-First Search) — Minimum Transit Path.
 *
 * Finds the path with the fewest edges (transitions/transits)
 * from source to destination, regardless of time or cost.
 *
 * Why BFS for minimum edges:
 * BFS explores nodes level by level (hops from source).
 * The first time we reach the destination, we guaranteed
 * the minimum number of hops (transits).
 *
 * Complexity: O(V + E)
 */
public class BFS {

    private Graph graph;

    public BFS(Graph graph) {
        this.graph = graph;
    }

    /**
     * Find minimum-transit path from source to destination.
     */
    public RouteResult findMinTransitPath(int sourceId, int destId) {
        int n = graph.getMaxNodes();

        boolean[] visited = new boolean[n];
        int[] parent = new int[n];

        for (int i = 0; i < n; i++) {
            parent[i] = -1;
        }

        visited[sourceId] = true;

        SimpleQueue queue = new SimpleQueue(n);
        queue.enqueue(sourceId);

        boolean found = false;

        while (!queue.isEmpty()) {
            int u = queue.dequeue();

            if (u == destId) {
                found = true;
                break;
            }

            for (Route r : graph.getNeighbors(u)) {
                int v = r.getToId();
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    queue.enqueue(v);
                }
            }
        }

        return buildResult(sourceId, destId, parent, found);
    }

    private RouteResult buildResult(int src, int dest, int[] parent, boolean found) {
        RouteResult result = new RouteResult();

        if (!found) {
            result.setFound(false);
            return result;
        }

        // Reconstruct path
        List<Integer> reversePath = new ArrayList<>();
        int cur = dest;
        while (cur != -1) {
            reversePath.add(cur);
            cur = parent[cur];
            if (reversePath.size() > graph.getMaxNodes())
                break;
        }

        List<Integer> path = new ArrayList<>();
        for (int i = reversePath.size() - 1; i >= 0; i--) {
            path.add(reversePath.get(i));
        }

        // Calculate time and cost along BFS path
        int totalTime = 0, totalCost = 0;
        for (int i = 0; i < path.size() - 1; i++) {
            int from = path.get(i);
            int to = path.get(i + 1);
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
