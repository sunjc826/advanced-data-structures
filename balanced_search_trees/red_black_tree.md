**Proposition** Coloring a height balanced tree in the following way produces a red black tree:
> In a height-balanced tree if one colors every node of odd height whose upper
neighbor is of even height red and all other nodes black, then it satisfies the
conditions of a red-black tree.

We can use a lemma. Suppose the root of a height-balanced tree is of height h. Denote the black height of a node to be the number of black nodes from itself to any of its leaves.
- If h = 2k is even, then the black height = k+1
- If h = 2k+1 is odd, then the black height = k or k+1 depending on parent.

Use these assumptions to prove results for when h = 2k + 2 and when h = 2k + 3.
Similar things can be shown if h' is odd. $\square$

> not all red-black trees are height-balanced; an additional restriction is that if a node is black and both of its lower neighbors are black, then at least one of their lower neighbors must be red

TODO.
