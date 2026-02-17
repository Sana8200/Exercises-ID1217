package MasterWorkerLatche;

import java.util.concurrent.CountDownLatch;

class Master {
    static final int N = 5;

    public static void main(String[] args) throws InterruptedException {
        // Start signal: initialized to 1, master counts down to let workers start
        CountDownLatch startSignal = new CountDownLatch(1);

        // done Signal: initialized to N, each worker counts down when done
        CountDownLatch doneSignal = new CountDownLatch(N);

        // Create and start worker threads (they'll bock on startSignal)
        for (int i = 0; i < N; i++) {
            new Thread(new Worker(startSignal, doneSignal, i)).start();
        }

        System.out.println("Master: preparing Worker...");
        Thread.sleep(1000); // doSomethingElse()

        System.out.println("Master: letting workers start!");
        startSignal.countDown(); // Count from 1 to 0 -> all workers unblock

        System.out.println("Master: doing other work while workers run...");

        doneSignal.await(); // Block until all N workers cound down
        System.out.println("Master: all workers finished!");

    }

}