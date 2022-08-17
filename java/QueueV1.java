package java;

public class QueueV1<T> implements BoundedQueue<T> {
    Object arr[];
    int base = 0;
    int rear = 0;
    int capacity; // the actual number of elements stored is capacity - 1

    QueueV1(int capacity) {
        this.capacity = capacity;
        this.arr = new Object[capacity];
    }

    @Override
    public boolean isEmpty() {
        return this.base == this.rear;
    }

    @Override
    public boolean isFull() {
        return this.base == (this.rear + 1) % this.capacity;
    }

    @Override
    public boolean enqueue(T item) {
        if (this.isFull())
            return false;

        this.arr[this.rear] = item;
        this.rear = (this.rear + 1) % this.capacity;
        return true;
    }

    @Override
    public T peek() {
        return (T) this.arr[(this.rear - 1) % this.capacity];
    }

    @Override
    public T dequeue() {
        this.rear = (this.rear - 1) % this.capacity;
        return (T) this.arr[this.rear];
    }
}
