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

public class boundedBuffer {

    // ═══════════════════════════════════════════════════════
    // SYNCHRONIZATION TOOLS
    // ═══════════════════════════════════════════════════════

    // The lock ensures only one thread access the buffer at a time 
    // Reentrant means the same thread can lock it multiple times without deadlocking itself
    final Lock lock = new ReentrantLock();



    // Producers wait here on the when buffer is full (will continue when not full)
    final Condition notFull = lock.newCondition();

    // Consumers wait here on this when buffer is empty (will continue when not empty)
    final Condition notEmpty = lock.newCondition();

    // ═══════════════════════════════════════════════════════
    // BUFFER DATA
    // ═══════════════════════════════════════════════════════
    final Object[] items;    // The actual buffer storage array
    int rear;                // Where to put next item, index where next item will be inserted 
    int front;               // Where to take next item, index where next item will be removed 
    int count;               // current number of items in the buffer
    int n;                   // Maximum capacity (count == n means full)(count == 0 means empty)

    // ═══════════════════════════════════════════════════════
    // CONSTRUCTOR
    // ═══════════════════════════════════════════════════════
    public boundedBuffer(int n) {  
        this.n = n;
        items = new Object[n];     // Object is the superclass of all classes, so we can store objects of any class and type
    }


    // Producer puts an item in the buffer
    public void put(Object x) throws InterruptedException {
        lock.lock();                   // 1. Get exclusive access, acquire the lock
        try {
            while (count == n){        // 2. Buffer full? (wait if buffer is full, so we don't overwrite existing items)
                notFull.await();       //    Wait for space
                // notFull.await() releases the lock(so others can work), puts this thread to sleep until signaled,
                // when signaled it re-acquires the lock before returning
            }   

            items[rear] = x;           // 3. Add item, puts item at rear index (position)
            rear = (rear + 1) % n;     //    Move rear forward (circular)
            count++;                   //    Increment count, one more item in buffer

            notEmpty.signal();         // 4. Tell consumers: not empty!
            // wake up one consumer who might be waiting on notEmpty condition

        } finally {
            lock.unlock();             // 5. ALWAYS release lock
            // unlocking must be in finally to ensure unlock even if exception occurs, preventing deadlocks
        }
    }


    /* This buffer actually can be accessed at the same time by producer and consumer, 
       because they access the different ends of the buffer 
    */

    // Consumer takes an item from the buffer
    public Object take() throws InterruptedException {
        lock.lock();                    // 1. Get exclusive access, acquire the lock
        try {
            while (count == 0){         // 2. Buffer empty? (wait if buffer is empty, so we don't read non-existing items)
                notEmpty.await();       //    Wait for items
                // nontEmpty.await() releases the lock(so producer can add), puts this thread to sleep until signaled,
                // when signaled it re-acquires the lock before returning
            }
            Object x = items[front];    // 3. Get item, removes item at front index (position)
        //  item[front] = null;         // Optional: Clear the slot for garbage collection (GC)
            front = (front + 1) % n;    //    Move front forward (circular)
            count--;                    //    Decrement count, one less item in buffer 

            notFull.signal();           // 4. Tell producers: not full!
            // Wake up one producer who might be waiting on notFull condition

            return x;

        } finally {
            // always release the lock
            lock.unlock();              // 5. ALWAYS release lock
        }
    }
}
