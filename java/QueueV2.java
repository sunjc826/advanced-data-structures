package java;

public class QueueV2<T> implements Queue<T> {
    class QueueNode {
        T item;
        QueueNode next = null;

        QueueNode(T item) {
            this.item = item;
        }
    }

    // delete here
    QueueNode front = null;
    // insert here
    QueueNode rear = null;

    @Override
    public boolean isEmpty() {
        return this.front == null;
    }

    @Override
    public void enqueue(T item) {
        QueueNode newNode = new QueueNode(item);

        if (this.isEmpty()) {
            this.front = this.rear = newNode;
        } else {
            this.rear.next = newNode;
            this.rear = newNode;
        }
    }

    @Override
    public T peek() {
        return this.front.item;
    }

    @Override
    public T dequeue() {
        T result = this.front.item;
        this.front = this.front.next;
        return result;
    }
}
