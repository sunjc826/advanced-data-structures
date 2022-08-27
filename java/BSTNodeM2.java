package java;

// Model 2 search tree
public class BSTNodeM2<K extends ComparableExt<K>, V> implements BST<K, V>
{
    BSTNodeM2<K, V> left;
    BSTNodeM2<K, V> right;
    K key;
    V value;

    public static <K extends ComparableExt<K>, V> BSTNodeM2<K, V> NewTree()
    {
        return new BSTNodeM2<>(null, null);
    }

    private BSTNodeM2(K key, V value)
    {
        this.key = key;
        this.value = value;
    }

    private void copy(BSTNodeM2<K, V> other)
    {
        this.key = other.key;
        this.left = other.left;
        this.right = other.right;
        this.value = other.value;
    }

    // Only applies to the root node
    @Override
    public boolean isEmpty()
    {
        return this.key == null;
    }

    @Override
    public V find(K key)
    {
        assert (key != null);
        if (this.isEmpty())
            return null;

        BSTNodeM2<K, V> current = this;
        while (current != null)
        {
            if (key.equals(current.key))
                return current.value;
            else if (key.lt(current.key))
                current = current.left;
            else
                current = current.right;
        }

        return null;
    }

    @Override
    public boolean insert(K key, V value)
    {
        assert (key != null);
        assert (value != null);

        if (this.isEmpty())
        {
            this.key = key;
            this.value = value;
            return true;
        }

        BSTNodeM2<K, V> current = this, parent = null;
        while (current != null)
        {
            parent = current;
            if (key.equals(current.key))
            {
                if (current.value != null)
                    return false;
                current.value = value;
                return true;
            }
            else if (key.lt(current.key))
                current = current.left;
            else
                current = current.right;
        }

        BSTNodeM2<K, V> newNode = new BSTNodeM2<>(key, value);
        if (key.lt(parent.key))
            parent.left = newNode;
        else
            parent.right = newNode;

        return true;
    }

    // By my understanding, the delete function is one of reasons to prefer the
    // Model 1 tree over the Model 2 tree. As we can see, the key is technically
    // still
    // existing inside the Model 2 tree after deletion.
    // We rely on the convention that we cannot store null inside a tree to indicate
    // that
    // the key, despite being there, is actually "non-existent".
    @Override
    public V delete(K key)
    {
        assert (key != null);
        if (this.isEmpty())
            return null;

        V value;

        BSTNodeM2<K, V> current = this, parent = null;
        while (current != null && !key.equals(current.key))
        {
            parent = current;
            if (key.lt(current.key))
                current = current.left;
            else
                current = current.right;
        }

        if (current == null)
            return null;

        value = current.value;
        if (current.left == null && current.right == null)
        {
            if (current == this)// root
                current.value = null;
            else
            {
                if (current == parent.left)
                    parent.left = null;
                else
                    parent.right = null;
            }
        }
        else if (current.left == null)
            current.copy(current.right);
        else if (current.right == null)
            current.copy(current.left);
        else
            current.value = null;

        return value;
    }

}
