**Proposition** The minimum number of leaves in a search tree of height $h$ is $m_h = h+1$.

**Proof 1** The inductive definition:
- Base case: $m_0 = 1 = 0 + 1$
- Inductive case: $m_h = m_{h-1} + \min\{m_i: 0\leq i\leq h-1\}$
Clearly, every non-empty tree has at least one leaf, and $m_0 = 1$, so $m_h = m_{h-1} + 1$, hence $m_h = h + m_0 = h + 1$. $\square$

**Proof 2** Define $N, L, n$ as in [models of trees](./models_of_trees.md).
- $2n + 1 = N$
- $N = L + n$
- $L = n+1 = \frac{N-1}{2} + 1$
The minimal number of interior nodes is $n = h$. The reason is that there must be a chain of nodes from the root whose length is $h+1$ (From depth 0 to depth $h$), and only 1 of them can be a leaf. Hence minimal $L = n+1 = h+1$. $\square$

**Proposition** A search tree with $n$ objects has height in $[\lceil \log n \rceil, n - 1]$.

**Proof** Let $h$ be the height of the tree. We have proven that the number of objects in the tree is tightly bounded by $[h+1, 2^h]$. Furthermore, $n\in [h+1, 2^h]$ so that $h+1\leq n\leq 2^h$.
Making $h$ the subject, we obtain $\log n\leq h\leq n-1$.
Equivalently, $\lceil \log n\rceil\leq h\leq n-1$. The bounds are tight as $n\in [h+1, 2^h]$ is tight. $\square$

**Proposition** A search tree for $n$ objects has average depth in $[\log n, \frac{(n-1)(n+2)}{2n}]$.

**Proof** Equivalently, we show that the total depth is bounded by $[n\log n, \frac{(n-1)(n+2)}{2}]$.

Intuitively, the upper bound can be attained by a linear chain of comparison nodes, to give the sum of depth of leaf nodes as $(1 + 2 + \dots + (n-1)) + (n-1) = \frac{n(n-1)}{2} + (n-1) = \frac{(n-1)(n+2)}{2}$.

Define $D_n$ as the maximum depth sum for $n$ objects. Then,
$$
D_n = n + \max\{D_i + D_{n-i} : 1\leq i\leq n-1\}
$$
where $D_1 = 0$

Define $d_n$ as the minimum depth sum for $n$ objects.
$$
d_n = n + \min\{d_i + d_{n-i} : 1\leq i\leq n-1\}
$$
where $d_1 = 0$

We solve the recursion for $D_n$ by induction. Assume $\forall k<n, D_k = \frac{(k-1)(k+2)}{2}$. Then $D_n = n+\max\{\frac{(i-1)(i+2)}{2} + \frac{(n-i-1)(n-i+2)}{2}:1\leq i\leq n-1\}$.
Maximizing $\frac{(i-1)(i+2)}{2} + \frac{(n-i-1)(n-i+2)}{2}$ is equivalent to maximizing $(i-1)(i+2) + (n-i-1)(n-i+2) = n^2 - 4 + 2i^2 - 2ni$ is equivalent to maximizing $i^2 - ni = i(i-n)$. Since $i(i-n)$ is a quadratic parabola with minimum at $\frac{n}{2}$, by symmetry the maximum is obtained at $\{1, n-1\}$, which is as far away from the line of symmetry as possible.
This gives $D_n = \frac{(n-1)(n+2)}{2}$.

For $d_n$, assume $\forall k<n, d_k\geq k\log k$.
$\frac{1}{2}k\log k + \frac{1}{2}(n-k)\log (n-k)\geq \frac{n}{2}\log \frac{n}{2}$ which gives $d_k + d_{n-k} = k\log k + (n-k)\log (n-k) = n\log \frac{n}{2}$.
Further, $d_n \geq n+n\log \frac{n}{2} = n\log n$. $\square$
