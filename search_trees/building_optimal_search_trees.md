The top down algorithm makes use of the regularities of the model 1 search tree. 

**Proposition** The key of an interior (comparison) node can be obtained from the key of the left-most descendant of the right subtree.

**Proof** Each interior node in a model 1 search tree has exactly 2 children. In particular, an interior node has a right child. This implies that the successor of a node must lie in its right subtree. Hence, it is sufficient to find the node with the smallest key in the right subtree, which is precisely the left-most descendant of the right subtree. (This is a leaf node.) Furthermore, it is sufficient to use the key of the aforementioned leaf node as the key of the interior node.
For clarity, we now denote the interior node as $v$ and the leaf node as $v'$.
The search space of $v$ is some interval $[a, b)$. The search space of the right subtree of $v$ is $[v.key, b)$. For $v'$ to be within the search space, we need $v.key\leq v'.key$, so this is a necessary condition for the tree to be valid. To see that it is valid for $v$ to be equal to $v'.key$, we simply make the observation that $v'.key$ is strictly greater than any node in the left subtree, so setting $v.key = v'.key$ would set the search space of the left subtree to $[a, v'.key)$, which must contains all leaves associated with the left subtree. $\square$

**Discussion** There is a good separation of responsibilites between the left and right child of a node. We define the following notation. For a node $v$,
- $left(v)$ is the left child
- $right(v)$ is the right child
- $desc(v)$ is the left-most descendant of subtree rooted at $v$
- $anc(v)$ is the lowest ancestor of $v$ from the root whose child leading to $v$ is a right child

Then, there are 2 main responsibilities associated with a certain node $v$.
1. Setting $v.key$ to $desc(right(v)).key$
2. Setting $anc(v).key$ to $desc(v).key$

Responsibility 1 of $v$ can be transformed to responsiblity 2 of $right(v)$, since $anc(right(v)) = v$.
Responsibility 2 of $v$ can be passed on to responsibility 2 of $left(v)$, since $anc(left(v)) = anc(v)$ and $desc(left(v)) = desc(v)$.

Hence, we have a very nice separation of responsibilities.

**Proposition** The fulfillment of the 2 responsibilities for all nodes ensure that the resulting tree has keys fully and correctly filled in. Furthermore, the algorithm `make_tree_top_down` achieves this.

**Intuitive idea of proof** We see that responsiblity 1 is sufficient to fully and correctly fill in all keys. Let $v$ be any interior node in the tree. Its responsibility 1 will be converted to the responsibility 2 of its right child (which exists). And the responsibility 2 of the right child will get propagated down its left descendants, eventually reaching its left-most descendant which is a leaf node. (In particular, responsibility 2 is never lost along the way!) Hence, responsibility 2 is guaranteed to be fulfilled. 
