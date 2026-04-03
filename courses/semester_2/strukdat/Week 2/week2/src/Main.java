public class Main {
    public static void main(String[] args) throws Exception {
        Car mobil_lamborghini = new Car("Lamborghini", "Revuelto", "yellow", 0);

        System.out.println(mobil_lamborghini.getBrand());
    }
}


class Car {
    // attribute anjay
    private String brand;
    private String type;
    private String color;
    private int speed;

    // constructor anjay
    public Car(){}
    public Car(String brand, String type, String color, int speed) {
        this.brand =  brand;
        this.type = type;
        this.color = color;
        this.speed = speed;
    }

    // method
    public void accelerate(int speedIncrease) {}
    public void brake() {}

    public String getBrand() {
        return brand;
    }

    public void setBrand(String brand) {
        this.brand = brand;
    }
}
