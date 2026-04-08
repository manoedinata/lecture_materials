// ABSTRAKSI
// Abstraksi adalah konsep dalam pemrograman yang memungkinkan kita untuk menyembunyikan
// detail implementasi dan hanya menampilkan fitur atau fungsi yang penting bagi pengguna.
// Dengan menggunakan abstraksi, kita dapat fokus pada apa yang dilakukan suatu objek,
// tanpa harus memikirkan bagaimana cara kerjanya secara internal.

public abstract class Vehicle {
    private String brand;
    private String model;

    public Vehicle(String brand, String model) {
        this.brand = brand;
        this.model = model;
    }

    // ENCAPSULATION
    // Encapsulation adalah konsep dalam pemrograman yang memungkinkan kita
    // untuk menyembunyikan data dan hanya memberikan akses melalui
    // method tertentu, yaitu getter dan setter.
    public String getBrand() {
        return brand;
    }
    public void setBrand(String brand) {
        this.brand = brand;
    }
    public String getModel() {
        return model;
    }
    public void setModel(String model) {
        this.model = model;
    }

    // METHODE ABSTRAK
    // Disini, startEngine() dan stopEngine() adalah method abstrak yang tidak memiliki implementasi.
    // Method ini harus diimplementasikan oleh class yang mewarisi class Vehicle.
    public abstract void startEngine();
    public abstract void stopEngine();

}
