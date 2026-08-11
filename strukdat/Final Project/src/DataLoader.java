
import graph.Graph;
import model.Route;
import model.Station;
import tree.Trie;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

/**
 * DataLoader — populates the graph with 25 Jakarta-area stations and 42 routes.
 * Dataset is self-created (not from external library).
 *
 * Station attributes: id, name, city, type, lat, lon
 * Route attributes: fromId, toId, timeMinutes, costRupiah, lineName,
 * transportType
 */
public class DataLoader {

    public static void load(Graph graph, Trie trie) {
        loadStations(graph, trie);
        loadRoutes(graph);
    }

    private static void loadStations(Graph graph, Trie trie) {
        String csvFile = "data/stations.csv";
        // String csvFile = "data/stations_jatim.csv";
        String line = "";
        String csvSplitBy = ",";

        try (BufferedReader br = new BufferedReader(new FileReader(csvFile))) {
            // Skip header line
            br.readLine();

            while ((line = br.readLine()) != null) {
                String[] data = line.split(csvSplitBy);
                if (data.length >= 6) {
                    int id = Integer.parseInt(data[0].trim());
                    String name = data[1].trim();
                    String city = data[2].trim();
                    String type = data[3].trim();
                    double lat = Double.parseDouble(data[4].trim());
                    double lon = Double.parseDouble(data[5].trim());

                    Station s = new Station(id, name, city, type, lat, lon);
                    graph.addStation(s);
                    trie.insert(s.getName(), s.getId());
                }
            }
        } catch (IOException e) {
            System.err.println("Error reading stations CSV file: " + e.getMessage());
        }
    }

    private static void loadRoutes(Graph graph) {
        String csvFile = "data/routes.csv";
        // String csvFile = "data/routes_jatim.csv";
        String line = "";
        String csvSplitBy = ",";

        try (BufferedReader br = new BufferedReader(new FileReader(csvFile))) {
            // Skip header line
            br.readLine();

            while ((line = br.readLine()) != null) {
                String[] data = line.split(csvSplitBy);
                if (data.length >= 5) {
                    int fromId = Integer.parseInt(data[0].trim());
                    int toId = Integer.parseInt(data[1].trim());
                    int time = Integer.parseInt(data[2].trim());
                    int cost = Integer.parseInt(data[3].trim());
                    String lineName = data[4].trim();
                    String transportType = "KRL"; // Fallback or default
                    if (lineName.contains("MRT")) {
                        transportType = "MRT";
                    } else if (lineName.contains("KA")) {
                        transportType = "KA";
                    } else if (lineName.contains("TransJakarta")) {
                        transportType = "BUS";
                    }

                    // Add routes in both directions as originally configured
                    graph.addRoute(new Route(fromId, toId, time, cost, lineName, transportType));
                    graph.addRoute(new Route(toId, fromId, time, cost, lineName, transportType));
                }
            }
        } catch (IOException e) {
            System.err.println("Error reading routes CSV file: " + e.getMessage());
        }
    }

    /** Helper: add route in both directions */
    private static void addBoth(Graph g, int a, int b, int time, int cost, String line) {
        g.addRoute(new Route(a, b, time, cost, line, "KRL"));
        g.addRoute(new Route(b, a, time, cost, line, "KRL"));
    }
}
