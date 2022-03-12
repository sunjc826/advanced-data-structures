**Proposition** A tree of height $h$ and model 1 contains $\leq 2^h$ objects.
For model 1 trees, counting the number of objects is equivalent to counting the number of leaves. Hence, we can replace all mentions of "objects" with "leaves".
**Proof 1** We use an inductive relation to obtain the upper bound. Let $M_h, h\in \mathbb{N}$ be the maximum number of objects of a model 1 tree of height $h$. Then
- Base case: $M_0 = 1 = 2^0$
- $\forall h\geq 1$, $M_h = M_{h-1} + \max\{M_i : 0\leq i\leq h-1\}$, where one of the left or right child must have height $h-1$ and the other has height $0\leq i\leq h-1$, so we need to take the maximum over these. Clearly, $M_h > M_{h-1}$ since one of its 2 children has $M_{h-1}$ objects and the other child has at least 1 object, i.e. $M_h$ is strictly increasing in $h$.
  This means that $M_h = 2M_{h-1} = 2^hM_0 = 2^h$
$\square$

**Proof 2** In the second proof, we consider the relationship between the number of leaves and the number of nodes in a model 1 tree. Let $L$ be the number of leaves, $N$ be the number of nodes, $n$ be the number of interior nodes. Then the following hold
- $2n + 1 = N$. The idea is that each interior node maps to the 2 of its children, and adding the root, we get exactly the number of nodes in the tree.
- $N = L + n$
- Combining these we get $L = n+1 = \frac{N-1}{2} + 1$. Note that we want to maximize $L$ so it is equivalent to maximize $N$. The maximimum number of nodes in a tree of height $h$ is $N=1 + 2 + \dots + 2^h = 2^{h+1} - 1$, i.e. a full binary tree. The reason is that every tree with height $\leq h$ must be a "subset" of a full binary tree, so a full binary tree is the upper bound in node count.
  Hence, we obtain $L = \frac{2^{h+1}-2}{2} + 1 = 2^h$. $\square$

A construction argument that a full binary tree is the upper bound in node count can be of the following form. Suppose a tree has height $\leq h$, and it is not a full binary tree. Then there exists a leaf node whose depth $<h$, so that we can branch off 2 children from it. The depth of the children $\leq h$, so the resulting tree's height is still within $h$, yet the number of nodes has increased by $2$. Applying this argument iteratively, we would obtain a full binary tree.

**Proposition** A tree of height $h$ and model 2 contains $\leq 2^{h+1}$ objects.
**Proof 1** We consider an inductive relation. Let $M_h, h\in \mathbb{N}$ be the maximum number of nodes in a model 2 tree. Then,
- Base case: $M_0 = 1 = 2^{0+1}-1$
- Inductive case: $\forall h\geq 1, M_h = 1 + M_{h-1} + \max\{M_i : 0\leq i\leq h-1\}$. By similar reasoning as above, we get $M_h = 2M_{h-1} + 1$ and so inductively we obtain $M_h = \sum_{0\leq i\leq h}2^i = 2^{h+1}-1$. $\square$

**Proof 2** The number of objects in a model 2 tree is equal to the number of nodes in the tree. We only need to define $N$ here, and the maximal value of $N = 2^{h+1} - 1$, the number of nodes in a full binary tree. $\square$

**Proposition** In a model 1 tree, no 2 comparison nodes can have the same key.

**Proof** We assume there are 2 comparison nodes $n_1, n_2$ (i.e. non-leaf nodes). For a model 1 tree to be valid, one comparison node must be the descendent of the other. WLOG, $n_2$ is the descendent of $n_1$.

We consider the case where $n_2$ is in the right subtree of $n_1$.
Let $[a, b)$ be the search space of the tree rooted at $n_1$. Let $k$ be the key of $n_1$. Then the left subtree of $n_1$ has search space $[a, k)$ and the right subtree has search space $[k, b)$.
The search space of $n_2$ is thus a subset $[c, d)\subseteq [k, b)$.
The left search space of $n_2$ is then $[c, k)\subseteq [k, k) = \emptyset$, which is not possible as all comparison nodes must have at least one leaf node in each of the left and right subtrees. But it is impossible for the left subtree of $n_2$ to have a leaf and a corresponding object since the search space is empty.

A similar proof can be given in the case where $n_2$ is in the left subtree of $n_1$.
$\square$

**Remark** In the other markdown documents in this folder, we will use model 1 by default unless specified.

**Remark** Each interior node in a model 1 tree must have 2 children. The reason is that interior nodes are comparison nodes, which are used to parition the search space. If an interior node has only 1 child, then the space partitioning is unnecessary and we can connect its sole child with its parent. 

