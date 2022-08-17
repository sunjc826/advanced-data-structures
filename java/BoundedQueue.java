package java;

public interface BoundedQueue<T> {
    boolean isEmpty();

    boolean isFull();

    // Returns false when the queue is full, true otherwise.
    boolean enqueue(T item);

    // Precondition: Queue is not empty.
    T peek();

    // Precondition: Queue is not empty.
    T dequeue();
}
