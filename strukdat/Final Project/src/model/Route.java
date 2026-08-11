package model;

public class Route {
    private int    fromId;
    private int    toId;
    private int    timeMinutes;   // weight 1
    private int    costRupiah;    // weight 2
    private String lineName;      // e.g., "KRL Commuter Line Bogor"
    private String transportType;

    public Route(int fromId, int toId, int timeMinutes, int costRupiah,
                 String lineName, String transportType) {
        this.fromId        = fromId;
        this.toId          = toId;
        this.timeMinutes   = timeMinutes;
        this.costRupiah    = costRupiah;
        this.lineName      = lineName;
        this.transportType = transportType;
    }

    public int    getFromId()        { return fromId; }
    public int    getToId()          { return toId; }
    public int    getTimeMinutes()   { return timeMinutes; }
    public int    getCostRupiah()    { return costRupiah; }
    public String getLineName()      { return lineName; }
    public String getTransportType(){ return transportType; }

    public void setTimeMinutes(int t) { this.timeMinutes = t; }
    public void setCostRupiah(int c)  { this.costRupiah = c; }

    @Override
    public String toString() {
        return String.format("Route(%d→%d | %d menit | Rp%,d | %s)",
                fromId, toId, timeMinutes, costRupiah, lineName);
    }
}
