package java;

public class QueueNodeV4<T> implements Queue<T> {
    T item = null;
    QueueNodeV4<T> next = null;

    public static <T> QueueNodeV4<T> NewQueue() {
        QueueNodeV4<T> queue = new QueueNodeV4<>();
        QueueNodeV4<T> placeholder = new QueueNodeV4<>();
        placeholder.next = placeholder;
        queue.next = placeholder;
        return queue;
    }

    private QueueNodeV4() {
    }

    private QueueNodeV4(T item) {
        this.item = item;
    }

    @Override
    public boolean isEmpty() {
        return this.next == this.next.next;
    }

    private void setBeforePlaceholder(QueueNodeV4<T> beforePlaceholder) {
        this.next = beforePlaceholder;
    }

    @Override
    public void enqueue(T item) {
        QueueNodeV4<T> newNode = new QueueNodeV4<>(item);
        QueueNodeV4<T> beforePlaceholder = this.next;
        QueueNodeV4<T> placeholder = beforePlaceholder.next;
        beforePlaceholder.next = newNode;
        newNode.next = placeholder;
        this.setBeforePlaceholder(newNode);
    }

    @Override
    public T peek() {
        return this.next.next.next.item;
    }

    @Override
    public T dequeue() {
        QueueNodeV4<T> beforePlaceholder = this.next;
        QueueNodeV4<T> placeholder = beforePlaceholder.next;
        QueueNodeV4<T> afterPlaceholder = placeholder.next;
        T result = afterPlaceholder.item;
        placeholder.next = afterPlaceholder.next;

        if (beforePlaceholder == afterPlaceholder)
            this.setBeforePlaceholder(placeholder);

        return result;
    }

}
