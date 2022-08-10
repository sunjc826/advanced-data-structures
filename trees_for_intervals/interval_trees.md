**Proposition** Consider the construction of the interval tree, in which we start by creating a search tree with the endpoints of the set of intervals. Given any interval $[a, b]$, whose endpoints $a, b$ exist as leaves in the tree, we can always find an interior node where we can store $[a, b]$. In other words, there exists an internal node $\texttt{n}$ such that
- $\texttt{n.key}\in [a, b]$
- $[a, b]$ is contained in the interval associated with $\texttt{n}$

**Proof idea** Consider the lowest common ancestor of the 2 leaves whose keys are $a$, $b$. $\square$
