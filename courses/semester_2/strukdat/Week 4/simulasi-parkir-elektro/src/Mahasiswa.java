// Kelas untuk Mahasiswa
class Mahasiswa extends Pengguna {
    protected String nrp; // NRP khusus untuk mahasiswa

    public Mahasiswa(String nama, String nrp) {
        super(nama, 2);
        this.nrp = nrp;

        // Gunakan sistem parkir terbatas untuk mahasiswa
        this.parkir = new LimitedAccessParkir();

        // Gunakan sistem gate dengan akses kondisional untuk mahasiswa
        this.gate = new ConditionalAccessGate();
    }

    @Override
    public void aksesGate() {
        gate.aksesGate(nrp); // Panggil metode aksesGate dengan NRP sebagai parameter
    }
}
