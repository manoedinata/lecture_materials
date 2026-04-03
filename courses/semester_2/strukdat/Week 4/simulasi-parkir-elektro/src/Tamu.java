// Kelas untuk Tamu
class Tamu extends Pengguna {
    public Tamu(String nama) {
        super(nama, 3);

        // tidak ada akses parkir untuk tamu
        this.parkir = new NoAccessParkir();

        // tidak ada akses gate untuk tamu
        this.gate = new NoAccessGate();
    }
}
