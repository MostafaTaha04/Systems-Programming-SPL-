package bgu.spl.mics;
 
import java.util.concurrent.TimeUnit;
 
public class Future<T> {
 
    private boolean isDone;
    private T result;  
    public Future() {
        isDone = false;
        result = null;
    }
 
    /**
     * Blocks until the result is available and returns it.
     */
    public synchronized  T get() {
            while (!isDone()) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
        }
        return result;
    }
 
    /**
     * Resolves the result of this Future object and notifies all waiting threads.
     */
    public synchronized void resolve(T result) {
            if (!isDone()) {  
                this.result = result;  
                this.isDone = true;  
                notifyAll();          
        }
    }
 
    /**
     * Checks if the result has been resolved.
     */
    public synchronized boolean isDone() {
        return isDone;
    }
 
    /**
     * Blocks for a given timeout and returns the result if available, or null otherwise.
     */
    public T get(long timeout, TimeUnit unit) {
        if (!isDone) {
            try {
                //unit.timedWait(this, timeout);
                Thread.sleep(timeout);
            } catch (InterruptedException e2) {
                e2.printStackTrace();
            }
        }
        if (isDone)
            return result;
        return null;
    }
}