package Replicated_Files;

import java.util.concurrent.*;

/**
 * FileServer — one instance = one server (e.g. Hospital[1])
 *
 * Pseudocode:
 *   module FileServer[myid = 1 to n]
 *     proc open(m)  { ... }
 *     proc close()  { ... }
 *     proc read()   { ... }
 *     proc write()  { ... }
 *     proc remote_write() { ... }
 */
public class fileServer {

    private final int myId;
    private final String name;
    private final lockProcess lock;

    // The actual file contents (just a string for simplicity)
    private volatile String fileContents;

    // "use" variable from pseudocode — READ or WRITE mode per client thread
    private final ThreadLocal<String> use = new ThreadLocal<>();

    // Reference to ALL servers (needed for write to reach remote copies)
    // Set after construction via setAllServers()
    private fileServer[] allServers;

    public fileServer(int myId, String name, String initialContents) {
        this.myId = myId;
        this.name = name;
        this.fileContents = initialContents;
        this.lock = new lockProcess(name);
    }

    public void setAllServers(fileServer[] servers) {
        this.allServers = servers;
    }

    // =========================================================
    // open(mode)
    //
    // Pseudocode:
    //   proc open(m) {
    //     if (m == READ)  call startread();
    //     else for [i=1 to n] call FileServer[i].startwrite();
    //   }
    // =========================================================
    public void open(String mode) throws InterruptedException {
        if (mode.equals("READ")) {
            log("open(READ)  → locking only myself");
            lock.startRead();                          // local lock only
            use.set("READ");
        } else {
            log("open(WRITE) → locking ALL " + allServers.length + " servers");
            for (fileServer server : allServers) {     // lock EVERY copy
                server.lock.startWrite();
            }
            use.set("WRITE");
        }
    }

    // =========================================================
    // close()
    //
    // Pseudocode:
    //   proc close() {
    //     if (use == READ)  send endread();
    //     else for [i=1 to n] send FileServer[i].endwrite();
    //   }
    //
    // "send" = async fire-and-forget, so we just call without waiting.
    // =========================================================
    public void close() {
        if ("READ".equals(use.get())) {
            log("close(READ)  → releasing local lock only");
            lock.endRead();
        } else {
            log("close(WRITE) → releasing ALL locks");
            for (fileServer server : allServers) {
                server.lock.endWrite();
            }
        }
        use.remove();
    }

    // =========================================================
    // read()
    //
    // Pseudocode:
    //   proc read(results) { read from local copy; return results; }
    // =========================================================
    public String read() {
        log("read() local copy → \"" + fileContents + "\"");
        return fileContents;
    }

    // =========================================================
    // write(value)
    //
    // Pseudocode:
    //   proc write(values) {
    //     write values into local copy;
    //     co [i=1 to n st i!=myid]
    //         call FileServer[i].remote_write(values);
    //   }
    //
    // "co" = concurrent, so we use threads for parallel remote updates.
    // =========================================================
    public void write(String newValue) throws InterruptedException {
        if ("READ".equals(use.get())) {
            System.out.println("[" + name + "] ERROR: file not opened for writing!");
            return;
        }

        log("write() → updating local copy to \"" + newValue + "\"");
        fileContents = newValue;    // write local copy first

        // co [i = 1 to n st i != myid] — concurrent remote updates
        ExecutorService pool = Executors.newCachedThreadPool();
        CountDownLatch latch = new CountDownLatch(allServers.length - 1);

        for (fileServer server : allServers) {
            if (server.myId != this.myId) {
                pool.submit(() -> {
                    try {
                        server.remoteWrite(newValue);
                    } finally {
                        latch.countDown();
                    }
                });
            }
        }

        latch.await();   // wait for ALL remote copies to update
        pool.shutdown();
        log("write() → all remote copies updated");
    }

    // =========================================================
    // remote_write() — called BY other servers on THIS server
    //
    // Pseudocode:
    //   proc remote_write(values) { write values into local copy; }
    // =========================================================
    public void remoteWrite(String newValue) {
        log("remoteWrite() ← received update → \"" + newValue + "\"");
        fileContents = newValue;
    }

    public String getName() { return name; }
    public String getContents() { return fileContents; }

    private void log(String msg) {
        System.out.println("[" + name + "] " + msg);
    }
}