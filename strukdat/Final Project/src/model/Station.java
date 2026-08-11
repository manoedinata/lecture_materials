package model;

public class Station {
    private int id;
    private String name;
    private String city;
    private String type;
    private double lat;
    private double lon;

    public Station(int id, String name, String city, String type, double lat, double lon) {
        this.id   = id;
        this.name = name;
        this.city = city;
        this.type = type;
        this.lat  = lat;
        this.lon  = lon;
    }

    public int    getId()   { return id; }
    public String getName() { return name; }
    public String getCity() { return city; }
    public String getType() { return type; }
    public double getLat()  { return lat; }
    public double getLon()  { return lon; }

    public void setName(String name) { this.name = name; }
    public void setCity(String city) { this.city = city; }
    public void setType(String type) { this.type = type; }

    @Override
    public String toString() {
        return String.format("[%2d] %-25s | %-20s | %-6s | (%.4f, %.4f)",
                id, name, city, type, lat, lon);
    }
}
