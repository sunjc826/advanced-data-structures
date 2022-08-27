package java;

public class StackNodeV4<T> implements Stack<T>
{
    Object arr[];
    int top = 0;
    int chunkCapacity;
    StackNodeV4<T> next = null;

    public static <T> StackNodeV4<T> NewStack(int chunkCapacity)
    {
        return new StackNodeV4<>(chunkCapacity);
    }

    private StackNodeV4(int chunkCapacity)
    {
        this.chunkCapacity = chunkCapacity;
        this.arr = new Object[chunkCapacity];
    }

    @Override
    public boolean isEmpty()
    {
        if (this.next != null)
            return true;

        return this.top == 0;
    }

    @Override
    public void push(T item)
    {
        if (this.top == this.chunkCapacity)
        {
            StackNodeV4<T> newChunk = new StackNodeV4<>(this.chunkCapacity);
            Object tmpArr[] = this.arr;
            this.arr = newChunk.arr;
            newChunk.arr = tmpArr;
            int tmpTop = this.top;
            this.top = newChunk.top;
            newChunk.top = tmpTop;

            newChunk.next = this.next;
            this.next = newChunk;
        }

        this.arr[this.top++] = item;
    }

    @Override
    public T peek()
    {
        return (T) ((this.top == 0) ? this.next.arr[this.next.top - 1] : this.arr[this.top - 1]);
    }

    @Override
    public T pop()
    {
        if (this.top == 0)
        {
            this.arr = this.next.arr;
            this.top = this.next.top;
            this.next = this.next.next;
        }

        return (T) this.arr[--this.top];
    }

}
