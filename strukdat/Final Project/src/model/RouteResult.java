package model;

import java.util.ArrayList;
import java.util.List;
import graph.Graph;

public class RouteResult {
    private List<Integer> path;
    private int totalTime;
    private int totalCost;
    private int numTransits;
    private boolean found;

    public RouteResult() {
        this.path = new ArrayList<>();
        this.found = false;
    }

    public List<Integer> getPath()      { return path; }
    public int  getTotalTime()          { return totalTime; }
    public int  getTotalCost()          { return totalCost; }
    public int  getNumTransits()        { return numTransits; }
    public boolean isFound()            { return found; }

    public void setPath(List<Integer> p){ this.path = p; }
    public void setTotalTime(int t)     { this.totalTime = t; }
    public void setTotalCost(int c)     { this.totalCost = c; }
    public void setNumTransits(int n)   { this.numTransits = n; }
    public void setFound(boolean f)     { this.found = f; }

    public void print(Graph graph) {
        if (!found) {
            System.out.println("   Tidak ada rute yang ditemukan.");
            return;
        }
        System.out.println("  Path   : " + buildPathString(graph));
        System.out.println("  Waktu  : " + totalTime + " menit");
        System.out.printf("  Biaya  : Rp%,d%n", totalCost);
        System.out.println("  Transit: " + numTransits + " kali");
    }

    private String buildPathString(Graph graph) {
        if (path.isEmpty()) return "-";
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < path.size(); i++) {
            Station s = graph.getStation(path.get(i));
            sb.append(s != null ? s.getName() : "?");
            if (i < path.size() - 1) sb.append(" → ");
        }
        return sb.toString();
    }
}
