import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class BoundedBuffer {

    // Synchronization tools
    final Lock lock = new ReentrantLock();

    // Producers wait here
    final Condition notFull = lock.newCondition();

    // Consumers wait here
    final Condition notEmpty = lock.newCondition();

    // Buffer storage
    final Object[] items; // The actual buffer
    int rear; // Where to put next item
    int front; // Where to take next item
    int count; // How many items currently
    int n; // Maximum capacity

    public BoundedBuffer(int n) {
        this.n = n;
        items = new Object[n];
    }


    
    public void put(Object x) throws InterruptedException {
        lock.lock(); // 1. Get exclusive access
        try {
            while (count == n) // 2. Buffer full?
                notFull.await(); // Wait for space

            items[rear] = x; // 3. Add item
            rear = (rear + 1) % n; // Move rear (circular)
            count++; // Increment count

            notEmpty.signal(); // 4. Tell consumers: not empty!

        } finally {
            lock.unlock(); // 5. ALWAYS release lock
        }
    }



    public Object take() throws InterruptedException {
        lock.lock(); // 1. Get exclusive access
        try {
            while (count == 0) // 2. Buffer empty?
                notEmpty.await(); // Wait for items

            Object x = items[front]; // 3. Get item
            front = (front + 1) % n; // Move front (circular)
            count--; // Decrement count

            notFull.signal(); // 4. Tell producers: not full!

            return x;

        } finally {
            lock.unlock(); // 5. ALWAYS release lock
        }
    }
}
