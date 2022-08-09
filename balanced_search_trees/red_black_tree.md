**Proposition** Coloring a height balanced tree in the following way produces a red black tree:
> In a height-balanced tree if one colors every node of odd height whose upper
neighbor is of even height red and all other nodes black, then it satisfies the
conditions of a red-black tree.

We can use a lemma. Suppose the root of a height-balanced tree is of height h. Denote the black height of a node to be the number of black nodes from itself to any of its leaves.
- If h is even, then the black height = h
- If h is odd, then the black height = h + 1

Inductively, we can then show that if h' is even, consider its child nodes. At least one child node must be of height h'-1 which is odd. Then by induction, the black height of the child node is h'-1. (Note: this is because that child node has odd height and its parent is of even height.) Hence, the black height of h' is h'.
Similar things can be shown if h' is odd. $\square$

> not all red-black trees are height-balanced; an additional restriction is that if a node is black and both of its lower neighbors are black, then at least one of their lower neighbors must be red

TODO.
