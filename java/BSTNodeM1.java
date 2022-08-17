package java;

// Model 1 search tree
// Note: Java does not support type unions, so both the `left`
// and the `value` attribute appear in a node as separate memory regions.
//
// Because there is no wrapper class Tree around a node, the root node
// has to be treated differently from other nodes.
//
// Another reason for the lack of symmetry is that the leaf nodes are different from interior nodes.
public class BSTNodeM1<K extends ComparableExt<K>, V> implements BST<K, V> {
    BSTNodeM1<K, V> left = null;
    BSTNodeM1<K, V> right = null;
    K key;
    V value;

    static <K extends ComparableExt<K>, V> BSTNodeM1<K, V> NewTree() {
        return new BSTNodeM1<>();
    }

    private BSTNodeM1() {
        this(null, null);
    }

    private BSTNodeM1(K key, V value) {
        this.key = key;
        this.value = value;
    }

    private void copy(BSTNodeM1<K, V> other) {
        this.key = other.key;
        this.left = other.left;
        this.right = other.right;
        this.value = other.value;
    }

    // This should only be used on the tree root.
    @Override
    public boolean isEmpty() {
        return this.left == null;
    }

    private boolean isLeaf() {
        return this.value != null;
    }

    @Override
    public V find(K key) {
        assert (key != null);
        if (this.isEmpty())
            return null;

        BSTNodeM1<K, V> current = this;
        while (!current.isLeaf()) {
            if (key.lt(current.key))
                current = current.left;
            else
                current = current.right;
        }

        return current.key.equals(key) ? current.value : null;
    }

    @Override
    public boolean insert(K key, V value) {
        assert (key != null);
        assert (value != null);
        if (this.isEmpty()) {
            this.key = key;
            this.value = value;
            return true;
        }

        BSTNodeM1<K, V> current = this;
        while (!current.isLeaf()) {
            if (key.lt(key))
                current = current.left;
            else
                current = current.right;
        }

        if (current.key.equals(key))
            return false;

        BSTNodeM1<K, V> newLeafNode = new BSTNodeM1<>(key, value);
        BSTNodeM1<K, V> oldLeafNode = new BSTNodeM1<>(current.key, current.value);
        current.value = null;

        if (key.lt(current.key)) {
            current.left = newLeafNode;
            current.right = oldLeafNode;
        } else {
            current.key = key;
            current.left = oldLeafNode;
            current.right = newLeafNode;
        }

        return true;
    }

    // Note that I do not check for empty root, see Preconditions
    @Override
    public V delete(K key) {
        assert (key != null);
        V value;
        if (this.isLeaf()) {
            if (!this.key.equals(key))
                return null;

            value = this.value;
            this.value = null;
            return value;
        }

        BSTNodeM1<K, V> current = this, parent = null;
        while (!current.isLeaf()) {
            parent = current;
            if (key.lt(current.key))
                current = current.left;
            else
                current = current.right;
        }

        if (!current.key.equals(key))
            return null;

        value = current.value;
        BSTNodeM1<K, V> sibling = (current == parent.left) ? parent.right : parent.left;
        parent.copy(sibling);
        return value;
    }
}
