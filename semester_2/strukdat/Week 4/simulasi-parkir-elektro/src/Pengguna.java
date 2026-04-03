// Abstract class untuk Pengguna
abstract class Pengguna {
    protected String nama;
    protected int role; // 1: Dosen, 2: Mahasiswa, 3: Tamu
    protected SistemParkir parkir; // Sistem parkir yang digunakan oleh pengguna
    protected SistemGate gate; // Sistem gate yang digunakan oleh pengguna

    public Pengguna(String nama, int role) {
        this.nama = nama;
        this.role = role;
    }

    public String getNama() { return nama; }
    public int getRole() { return role; }

    // Special Setter untuk mengubah hak parkir secara dinamis
    public void setSistemParkir(SistemParkir newParkir) {
        this.parkir = newParkir;
    }

    public void parkir() { parkir.aksesParkir(); }
    public void aksesGate() { gate.aksesGate(); }

}
