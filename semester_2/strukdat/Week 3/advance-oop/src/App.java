

public class App {
    public static void main(String[] args) throws Exception {
        // POLYMORPHISM
        // Polymorphism adalah konsep dalam pemrograman yang memungkinkan objek
        // dari class yang berbeda untuk diperlakukan sebagai objek dari class yang sama.
        // Dalam contoh ini, kita dapat menggunakan referensi dari class Vehicle.
        Car myCar = new Car("Toyota", "Corolla");
        Motorcycle myMotorcycle = new Motorcycle("Honda", "CBR500R");
        System.out.println("");
        
        // Kita dapat memanggil method startEngine() dan stopEngine()
        // pada objek myCar dan myMotorcycle, meskipun mereka adalah objek dari class yang berbeda.
        // Ini menunjukkan bahwa kita dapat menggunakan objek dari class yang berbeda
        // dengan cara yang sama, karena mereka mewarisi method yang sama dari class Vehicle.
        myCar.startEngine();
        myCar.stopEngine();
        System.out.println("");
        
        myMotorcycle.startEngine();
        myMotorcycle.stopEngine();

        myCar.payQRIS(10000);

        System.out.println("");
    }
}
