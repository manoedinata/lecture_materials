public class Car extends Vehicle {
    // QRISPayment
    protected QRISPayment qris = new QRISPayment();

    public Car(String brand, String model) {
        super(brand, model);
        System.out.println("[CAR CREATED] " + getBrand() + " " + getModel());
    }

    // Override method startEngine() dan stopEngine()
    // yang didefinisikan di class Vehicle.
    @Override
    public void startEngine() {
        System.out.println("[CAR START] Engine started for " + getBrand() + " " + getModel());
    }
    @Override
    public void stopEngine() {
        System.out.println("[CAR STOP] Engine stopped for " + getBrand() + " " + getModel());
    }

    public void payQRIS(double amount) {
        qris.payToll(amount);
    }

}
