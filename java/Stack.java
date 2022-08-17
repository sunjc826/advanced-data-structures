package java;

public interface Stack<T> {
    boolean isEmpty();

    void push(T item);

    // Precondition: Stack is not empty.
    T peek();

    // Precondition: Stack is not empty.
    T pop();
}
