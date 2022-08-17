package java;

// A search tree mapping keys of type K to objects of type V that doesn't allow duplicate keys.
public interface BST<K extends Comparable<K>, V> {
    boolean isEmpty();

    // Precondition: key is not null.
    V find(K key);

    // Precondition: `key` is not null, `value` is not null.
    // Returns false if insertion failed, e.g. due to duplicate keys.
    boolean insert(K key, V value);

    // Precondition: Tree is not empty. `key` is not null.
    // Returns null if key is non existent in the structure.
    V delete(K key);
}
