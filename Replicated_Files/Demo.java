package Replicated_Files;

/**
 * Demo — simulates doctors at 3 hospitals concurrently reading/writing
 * a shared patient medical record.
 *
 * Server layout:
 *   Hospital[1] = Stockholm  (myId=1)
 *   Hospital[2] = Gothenburg (myId=2)
 *   Hospital[3] = Malmo      (myId=3)
 */
public class Demo {

    public static void main(String[] args) throws InterruptedException {

        // ---- Setup: 3 servers, all holding same initial record ----
        fileServer stockholm  = new fileServer(1, "Stockholm",  "Medication: Aspirin 100mg");
        fileServer gothenburg = new fileServer(2, "Gothenburg", "Medication: Aspirin 100mg");
        fileServer malmo      = new fileServer(3, "Malmo",      "Medication: Aspirin 100mg");

        fileServer[] allServers = { stockholm, gothenburg, malmo };
        stockholm.setAllServers(allServers);
        gothenburg.setAllServers(allServers);
        malmo.setAllServers(allServers);

        System.out.println("=== SCENARIO 1: Two doctors READ concurrently ===\n");

        // Doctor A reads from Stockholm, Doctor B reads from Gothenburg — BOTH at same time
        Thread doctorA = new Thread(() -> {
            try {
                System.out.println("[Dr.A] Opening record for READ at Stockholm");
                stockholm.open("READ");
                String record = stockholm.read();
                System.out.println("[Dr.A] Got record: " + record);
                Thread.sleep(500);  // simulate reading time
                stockholm.close();
                System.out.println("[Dr.A] Done reading");
            } catch (InterruptedException e) { Thread.currentThread().interrupt(); }
        }, "DoctorA");

        Thread doctorB = new Thread(() -> {
            try {
                System.out.println("[Dr.B] Opening record for READ at Gothenburg");
                gothenburg.open("READ");
                String record = gothenburg.read();
                System.out.println("[Dr.B] Got record: " + record);
                Thread.sleep(500);
                gothenburg.close();
                System.out.println("[Dr.B] Done reading");
            } catch (InterruptedException e) { Thread.currentThread().interrupt(); }
        }, "DoctorB");

        doctorA.start();
        doctorB.start();
        doctorA.join();
        doctorB.join();

        System.out.println("\n=== SCENARIO 2: Doctor WRITES (blocks any concurrent readers) ===\n");

        // Doctor C wants to update the record from Malmo
        // This locks ALL THREE hospitals before writing
        Thread doctorC = new Thread(() -> {
            try {
                System.out.println("[Dr.C] Opening record for WRITE at Malmo");
                malmo.open("WRITE");
                malmo.write("Medication: Ibuprofen 400mg");  // updates all copies
                malmo.close();
                System.out.println("[Dr.C] Done writing");
            } catch (InterruptedException e) { Thread.currentThread().interrupt(); }
        }, "DoctorC");

        // Doctor D tries to read from Stockholm while C is writing — should BLOCK
        Thread doctorD = new Thread(() -> {
            try {
                Thread.sleep(50); // slight delay so C gets the write lock first
                System.out.println("[Dr.D] Trying to READ at Stockholm (should wait for Dr.C's write to finish)");
                stockholm.open("READ");
                String record = stockholm.read();
                System.out.println("[Dr.D] Got record: " + record);
                stockholm.close();
                System.out.println("[Dr.D] Done reading");
            } catch (InterruptedException e) { Thread.currentThread().interrupt(); }
        }, "DoctorD");

        doctorC.start();
        doctorD.start();
        doctorC.join();
        doctorD.join();

        System.out.println("\n=== Final state of all copies ===");
        for (fileServer s : allServers) {
            System.out.println(s.getName() + ": " + s.getContents());
        }
    }
}