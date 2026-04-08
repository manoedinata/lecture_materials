// Interface untuk Sistem Parkir
interface SistemParkir {
    void aksesParkir();
}

// Implementasi Sistem Parkir untuk Dosen
class FullAccessParkir implements SistemParkir {
    @Override
    public void aksesParkir() {
        System.out.println("Akses parkir + VIP.");
    }
}

// Implementasi Sistem Parkir untuk Mahasiswa
class LimitedAccessParkir implements SistemParkir {
    @Override
    public void aksesParkir() {
        System.out.println("Akses parkir saja.");
    }
}

// Implementasi Sistem Parkir dengan akses nol
class NoAccessParkir implements SistemParkir {
    @Override
    public void aksesParkir() {
        System.out.println("Tidak memiliki akses parkir.");
    }
}
