// Interface untuk boleh tidaknya pintu gate dibuka
interface SistemGate {
    default void aksesGate() {
        System.out.println("Metode ini tidak didukung di gate ini.");
    }
    
    default void aksesGate(String nrp) {
        System.out.println("Metode dengan NRP tidak didukung di gate ini.");
    }
}

class FullAccessGate implements SistemGate {
    @Override
    public void aksesGate() {
        System.out.println("Akses gate diizinkan.");
    }
}

class ConditionalAccessGate implements SistemGate {
    @Override
    public void aksesGate(String nrp) {
        // Check if NRP starts with:
        // [5022, 5023, 5024, 5027]
        if (nrp.startsWith("5022") || nrp.startsWith("5023") || nrp.startsWith("5024") || nrp.startsWith("5027")) {
            System.out.println("Akses gate diizinkan untuk NRP: " + nrp);
        } else {
            System.out.println("Akses gate dilarang.");
        }
    }
}

class NoAccessGate implements SistemGate {
    @Override
    public void aksesGate() {
        System.out.println("Akses gate dilarang.");
    }
}
