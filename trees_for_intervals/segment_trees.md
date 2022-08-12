For any model 1 binary search tree constructed using the method described in [search_tree_v1](../search_tree_v1.c), we can guarantee that a canonical decomposition exists for any interval $[a, b)$, where $a < b$ and both $a, b$ are key values in the leaves.
This guarantee hinges on this property of our construction.
**Proposition** Let T be any BST constructed in the aforementioned manner. Let v be the value of any value in the leaf nodes *that is not the smallest leaf node*, i.e. not the leftmost. Then the following properties hold:
1. The value $v$ appears precisely twice in the tree, once in an interior node, once in a leaf.
2. The interior node in which $v$ appears is the lowest ancestor on the path from the root to v's leaf whose child on this path is a right child.

As for the leftmost leaf node, its key does not appear in any interior node.

**Proposition (Equivalent)** For any interior node in the tree, its key value can be found by the leftmost node of its right subtree. 

**Proof idea** The proof is by structural induction on a search tree. Specifically, we need to show that
- An empty tree upholds these properties (trivial)
- Supposing a tree T upholds these properties, then any insertion/deletion will maintain these properties.

We can easily extend this induction to the height-balanced tree, as we just need to show
- Any rotation (left/right) will maintain these properties.
$\square$
