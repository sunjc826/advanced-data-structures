package java;

public class StackV2<T> implements BoundedStack<T>
{
    Object arr[];
    int top = 0;
    int capacity;

    public StackV2(int capacity)
    {
        this.capacity = capacity;
        this.arr = new Object[capacity];
    }

    @Override
    public boolean isEmpty()
    {
        return this.top == 0;
    }

    @Override
    public boolean isFull()
    {
        return this.top == this.capacity;
    }

    @Override
    public boolean push(T item)
    {
        if (this.isFull())
            return false;

        arr[top++] = item;
        return true;
    }

    @Override
    public T peek()
    {
        return (T) this.arr[this.top - 1];
    }

    @Override
    public T pop()
    {
        return (T) this.arr[--this.top];
    }
}
