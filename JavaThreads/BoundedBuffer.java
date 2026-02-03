import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/* The Storage (Circular Buffer) : The code uses an array items to store the objects, but it treats it like a circle.
   rear: Where the next item goes IN.
   front: Where the next item comes OUT.
   count: How many items are currently inside.
   The Math Trick: (rear + 1) % n This is what makes it circular. 
   If the buffer size is 4, indices go 0, 1, 2, 3. When you reach 3, the math (3 + 1) % 4 equals 0. 
   The pointer wraps back to the start! 
*/

public class BoundedBuffer {

    // Synchronization tools
    final Lock lock = new ReentrantLock();

    // Producers wait here
    final Condition notFull = lock.newCondition();

    // Consumers wait here
    final Condition notEmpty = lock.newCondition();

    // Buffer storage
    final Object[] items; // The actual buffer
    int rear;     // Where to put next item
    int front;    // Where to take next item
    int count;    // How many items currently in buffer
    int n;        // Maximum capacity (count == n means full)(count == 0 means empty)

    public BoundedBuffer(int n) {  // Constructor
        this.n = n;
        items = new Object[n];     // Object is the superclass of all classes, so we can store objects of any class and type
    }


    // Producer puts an item in the buffer
    public void put(Object x) throws InterruptedException {
        lock.lock();                   // 1. Get exclusive access
        try {
            while (count == n)         // 2. Buffer full?
                notFull.await();       //    Wait for space

            items[rear] = x;           // 3. Add item
            rear = (rear + 1) % n;     //    Move rear (circular)
            count++;                   //    Increment count

            notEmpty.signal();         // 4. Tell consumers: not empty!

        } finally {
            lock.unlock();             // 5. ALWAYS release lock
        }
    }


    /* This buffer actually can be accessed at the same time by producer and consumer, 
       because they access the different ends of the buffer 
    */

    // Consumer takes an item from the buffer
    public Object take() throws InterruptedException {
        lock.lock();                    // 1. Get exclusive access
        try {
            while (count == 0)          // 2. Buffer empty?
                notEmpty.await();       //    Wait for items

            Object x = items[front];    // 3. Get item
            front = (front + 1) % n;    //    Move front (circular)
            count--;                    //    Decrement count

            notFull.signal();           // 4. Tell producers: not full!

            return x;

        } finally {
            lock.unlock();              // 5. ALWAYS release lock
        }
    }
}
