package java;

public interface BoundedStack<T>
{
    boolean isEmpty();

    boolean isFull();

    // Returns false when the stack is full, true otherwise.
    boolean push(T item);

    // Precondition: Stack is not empty.
    T peek();

    // Precondition: Stack is not empty.
    T pop();
}
