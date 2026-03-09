package Replicated_Files;

public class lockProcess {

    private final String serverName;
    private int nr = 0;  // number of active readers
    private int nw = 0;  // number of active writers (0 or 1)

    public lockProcess(String serverName) {
        this.serverName = serverName;
    }

    // startread() — blocks until nw == 0
    public synchronized void startRead() throws InterruptedException {
        while (nw != 0) {          // guard: nw == 0
            wait();
        }
        nr++;
        log("startRead granted   | nr=" + nr + " nw=" + nw);
    }

    // endread() — always accepted immediately
    public synchronized void endRead() {
        nr--;
        log("endRead             | nr=" + nr + " nw=" + nw);
        notifyAll();               // a waiting writer might now proceed
    }

    // startwrite() — blocks until nr == 0 AND nw == 0, Invariant: (nr == 0 || nw == 0) && nw <= 1
    public synchronized void startWrite() throws InterruptedException {
        while (nr != 0 || nw != 0) {   // guard: nr==0 && nw==0
            wait();
        }
        nw++;
        log("startWrite granted  | nr=" + nr + " nw=" + nw);
    }

    // endwrite() — always accepted immediately
    public synchronized void endWrite() {
        nw--;
        log("endWrite            | nr=" + nr + " nw=" + nw);
        notifyAll();               // waiting readers/writers can now proceed
    }

    private void log(String msg) {
        System.out.println("[Lock:" + serverName + "] " + msg);
    }
}