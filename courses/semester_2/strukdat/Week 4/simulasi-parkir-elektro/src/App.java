// Simulasi Sistem Parkir Gate di Teknik Elektro ITS

public class App {
    public static void main(String[] args) throws Exception {

        Dosen dosen = new Dosen("Aris");
        Mahasiswa mahasiswa = new Mahasiswa("Hendra", "5027251051");
        Tamu tamu = new Tamu("Bambang Pramujati");

        System.out.println("Dosen:");
        System.out.println(dosen.getNama());
        dosen.aksesGate();
        dosen.parkir();

        System.out.println("\nMahasiswa:");
        System.out.println(mahasiswa.getNama());
        mahasiswa.aksesGate();
        mahasiswa.parkir();

        System.out.println("   ... eh tiba-tiba Mahasiswa diupgrade akses parkirnya!");
        mahasiswa.setSistemParkir(new FullAccessParkir());
        mahasiswa.parkir();

        System.out.println("\nTamu:");
        System.out.println(tamu.getNama());
        tamu.aksesGate();
        tamu.parkir();
    }
}
