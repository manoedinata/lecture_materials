## 4 Pillars of OOP

Di programming berorientasi objek (OOP), terdapat empat pilar utama yang menjadi dasar dalam pengembangan perangkat lunak:

### 1. Encapsulation (Enkapsulasi)

Enkapsulasi adalah konsep menyembunyikan detail implementasi suatu objek dan hanya menyediakan antarmuka yang diperlukan untuk berinteraksi dengan objek tersebut. Ini membantu menjaga integritas data dan mencegah akses langsung ke atribut atau metode yang tidak diinginkan.

```java
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
```

### 2. Inheritance (Pewarisan)

Pewarisan memungkinkan sebuah kelas untuk mewarisi sifat dan perilaku dari kelas lain. Kelas yang mewarisi disebut subclass atau child class, sedangkan kelas yang diwarisi disebut superclass atau parent class. Ini mempromosikan penggunaan kembali kode dan memudahkan pemeliharaan.

```java
// INHERITANCE
// Inheritance adalah konsep dalam pemrograman yang memungkinkan kita
// untuk membuat class baru berdasarkan class yang sudah ada. Class baru ini
// disebut subclass atau child class,

public class Car extends Vehicle {
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

}
```

### 3. Polymorphism (Polimorfisme)

Polimorfisme adalah kemampuan suatu objek untuk mengambil banyak bentuk. Dalam OOP, ini berarti bahwa objek dari kelas yang berbeda dapat diperlakukan sebagai objek dari kelas yang sama melalui pewarisan. Polimorfisme memungkinkan metode yang sama untuk berperilaku berbeda pada kelas yang berbeda.

```java
// POLYMORPHISM
// Polymorphism adalah konsep dalam pemrograman yang memungkinkan objek
// dari class yang berbeda untuk diperlakukan sebagai objek dari class yang sama.
// Dalam contoh ini, kita dapat menggunakan referensi dari class Vehicle.
Vehicle myCar = new Car("Toyota", "Corolla");
Vehicle myMotorcycle = new Motorcycle("Honda", "CBR500R");
```

### 4. Abstraction (Abstraksi)

Abstraksi adalah konsep menyembunyikan detail kompleks dan hanya menampilkan fitur penting dari suatu objek. Ini membantu dalam mengurangi kompleksitas dan meningkatkan efisiensi dengan fokus pada apa yang dilakukan objek daripada bagaimana cara kerjanya. Abstraksi sering dicapai melalui penggunaan kelas abstrak atau antarmuka.

```java
// Abstract class
abstract class Vehicle {
  // Abstract method (does not have a body)
  public abstract void startEngine();
  public abstract void stopEngine();
}

// Subclass (inherit from Vehicle)
class Car extends Vehicle {

  // Implementasi metode abstrak
  public void startEngine() {
    System.out.println("[CAR START] Engine started for " + brand + " " + model);
  }

  public void stopEngine() {
    System.out.println("[CAR STOP] Engine stopped for " + brand + " " + model);
  }
}
```

