// Kelas untuk Dosen
class Dosen extends Pengguna {
    public Dosen(String nama) {
        super(nama, 1);

        // Gunakan sistem parkir penuh untuk dosen
        this.parkir = new FullAccessParkir();

        // Gunakan sistem gate penuh untuk dosen
        this.gate = new FullAccessGate();
    }
}
