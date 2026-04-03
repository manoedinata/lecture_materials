// INHERITANCE
// Inheritance adalah konsep dalam pemrograman yang memungkinkan kita
// untuk membuat class baru berdasarkan class yang sudah ada. Class baru ini
// disebut subclass atau child class,

class Motorcycle extends Vehicle {
    public Motorcycle(String brand, String model) {
        super(brand, model);
        System.out.println("[MOTORCYCLE CREATED] " + getBrand() + " " + getModel());
    }

    // Override method startEngine() dan stopEngine()
    // yang didefinisikan di class Vehicle.
    @Override
    public void startEngine() {
        System.out.println("[MOTORCYCLE START] Engine started for " + getBrand() + " " + getModel());
    }
    @Override
    public void stopEngine() {
        System.out.println("[MOTORCYCLE STOP] Engine stopped for " + getBrand() + " " + getModel());
    }
}
