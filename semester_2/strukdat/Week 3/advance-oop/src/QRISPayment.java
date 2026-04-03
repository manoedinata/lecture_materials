interface ETollPayment {
    void payToll(double amount);
}

public class QRISPayment implements ETollPayment {
    @Override
    public void payToll(double amount) {
        System.out.println("PAYING WITH QRIS");
    }
}
