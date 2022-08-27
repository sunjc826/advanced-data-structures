package java;

public class StackNodeV3<T> implements Stack<T>
{
    private T item = null;
    private StackNodeV3<T> next = null;

    public static <T> StackNodeV3<T> NewStack()
    {
        return new StackNodeV3<>();
    }

    private StackNodeV3()
    {}

    private StackNodeV3(T item)
    {
        this.item = item;
    }

    @Override
    public boolean isEmpty()
    {
        return this.next == null;
    }

    @Override
    public void push(T item)
    {
        StackNodeV3<T> newNode = new StackNodeV3<>(item);
        newNode.next = this.next;
        this.next = newNode;
    }

    @Override
    public T peek()
    {
        return this.next.item;
    }

    // Precondition: Stack must be non-empty
    @Override
    public T pop()
    {
        T result = this.next.item;
        this.next = this.next.next;
        return result;
    }
}
