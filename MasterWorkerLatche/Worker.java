package MasterWorkerLatche;

import java.util.concurrent.CountDownLatch;

public class Worker implements Runnable {
    private final CountDownLatch startSignal, doenSignal;
    private final int id;

    Worker(CountDownLatch startSignal, CountDownLatch doneSignal, int id){
        this.startSignal = startSignal;
        this.doenSignal = doneSignal;
        this.id = id;
    }

    public void run(){
        try{
            startSignal.await();     // Wait until master gives the start signal

            // Do actual work
            System.out.println("Worker " + id + ": doing work...");
            Thread.sleep((long)(Math.random() * 2000)); // Simulate work by sleeping for a random time
            System.out.println("Worker " + id + ": done!");

            doenSignal.countDown();     // Signal that this worker is done
         } catch (InterruptedException e) {
        }
    }
}
