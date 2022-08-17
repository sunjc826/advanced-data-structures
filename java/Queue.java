package java;

public interface Queue<T> {
    boolean isEmpty();

    void enqueue(T item);

    // Precondition: Queue is not empty.
    T peek();

    // Precondition: Queue is not empty.
    T dequeue();
}
