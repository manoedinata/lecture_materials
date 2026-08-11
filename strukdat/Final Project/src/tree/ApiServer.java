package server;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;
import graph.BFS;
import graph.Dijkstra;
import graph.Graph;
import model.RouteResult;
import model.Station;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.nio.file.Files;
import java.util.List;

/**
 * Lightweight API Server to serve map visualization endpoints.
 * Built natively with Java's com.sun.net.httpserver (No external libraries).
 */
public class ApiServer {

    private Graph graph;

    public ApiServer(Graph graph) {
        this.graph = graph;
    }

    public void start(int port) throws IOException {
        HttpServer server = HttpServer.create(new InetSocketAddress(port), 0);

        // Register API endpoints mapped in index.html
        server.createContext("/", new IndexHandler());
        server.createContext("/api/stations", new StationsHandler());
        server.createContext("/api/route", new RouteHandler());

        server.setExecutor(null); // creates a default executor
        server.start();
        System.out.println("API Backend Server started dynamically on port: " + port);
        System.out.println("You can now open index.html in your browser!");
    }

    // =========================================================
    // HANDLER: / (Serve index.html)
    // =========================================================
    private class IndexHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            // Only serve the root path
            if (!exchange.getRequestURI().getPath().equals("/")) {
                String response = "404 Not Found";
                exchange.sendResponseHeaders(404, response.length());
                try (OutputStream os = exchange.getResponseBody()) {
                    os.write(response.getBytes());
                }
                return;
            }

            File file = new File("src/tree/index.html");
            if (!file.exists()) {
                String response = "index.html not found at src/tree/index.html";
                exchange.sendResponseHeaders(404, response.length());
                try (OutputStream os = exchange.getResponseBody()) {
                    os.write(response.getBytes());
                }
                return;
            }

            byte[] response = Files.readAllBytes(file.toPath());
            exchange.getResponseHeaders().add("Content-Type", "text/html");
            exchange.sendResponseHeaders(200, response.length);
            try (OutputStream os = exchange.getResponseBody()) {
                os.write(response);
            }
        }
    }

    // =========================================================
    // HANDLER: /api/stations
    // =========================================================
    private class StationsHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            // Allow cross-origin requests for local HTML testing
            exchange.getResponseHeaders().add("Access-Control-Allow-Origin", "*");
            exchange.getResponseHeaders().add("Content-Type", "application/json");

            StringBuilder json = new StringBuilder("[\n");
            boolean first = true;

            for (int i = 0; i < graph.getMaxNodes(); i++) {
                Station s = graph.getStation(i);
                if (s != null) {
                    if (!first)
                        json.append(",\n");
                    first = false;

                    // Default coordinates near Jakarta in case Station doesn't map them explicitly
                    double lat = -6.2088 + (Math.random() * 0.1 - 0.05);
                    double lon = 106.8456 + (Math.random() * 0.1 - 0.05);

                    try {
                        lat = (double) s.getClass().getMethod("getLat").invoke(s);
                        lon = (double) s.getClass().getMethod("getLon").invoke(s);
                    } catch (Exception ignored) {
                    }

                    json.append(String.format("  {\"id\": %d, \"name\": \"%s\", \"lat\": %f, \"lon\": %f}",
                            s.getId(), s.getName(), lat, lon));
                }
            }
            json.append("\n]");

            byte[] response = json.toString().getBytes();
            exchange.sendResponseHeaders(200, response.length);
            try (OutputStream os = exchange.getResponseBody()) {
                os.write(response);
            }
        }
    }

    // =========================================================
    // HANDLER: /api/route?src={id}&dest={id}&mode={mode}
    // =========================================================
    private class RouteHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            exchange.getResponseHeaders().add("Access-Control-Allow-Origin", "*");
            exchange.getResponseHeaders().add("Content-Type", "application/json");

            String query = exchange.getRequestURI().getQuery();
            int src = -1, dest = -1;
            String mode = "time";

            // Manual URL query parser
            if (query != null) {
                for (String param : query.split("&")) {
                    String[] pair = param.split("=");
                    if (pair.length == 2) {
                        if (pair[0].equals("src"))
                            src = Integer.parseInt(pair[1]);
                        else if (pair[0].equals("dest"))
                            dest = Integer.parseInt(pair[1]);
                        else if (pair[0].equals("mode"))
                            mode = pair[1];
                    }
                }
            }

            RouteResult result = null;
            if (src != -1 && dest != -1) {
                if (mode.equals("transit")) {
                    result = new BFS(graph).findMinTransitPath(src, dest);
                } else {
                    int dMode = mode.equals("cost") ? Dijkstra.MODE_COST : Dijkstra.MODE_TIME;
                    result = new Dijkstra(graph).findShortestPath(src, dest, dMode);
                }
            }

            String jsonResponse = result == null || !result.isFound() ? "{\"found\": false}"
                    : String.format("{\"found\": true, \"time\": %d, \"cost\": %d, \"transits\": %d, \"path\": %s}",
                            result.getTotalTime(), result.getTotalCost(), result.getNumTransits(),
                            result.getPath().toString());

            byte[] responseBytes = jsonResponse.getBytes();
            exchange.sendResponseHeaders(200, responseBytes.length);
            try (OutputStream os = exchange.getResponseBody()) {
                os.write(responseBytes);
            }
        }
    }
}