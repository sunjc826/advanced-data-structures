**Discussion** When proving the effectiveness of balanced search trees, it is a heuristic to provide a lower bound for the number of leaves given height. Precisely stated, let $n$ be the leaf count and $h$ the height. Let $g$ be some function such that $n\geq g(h)$. i.e given $h$, we have a lower bound for $n$. This is equivalent to $h\leq g^{-1}(n)$ (We assume an inverse of some sort exists.) Then this means that given $n$, we have an upper bound for $h$. And if this upper bound is good, this suggests that the balanced search tree is effective.


**Pg. 58**
> rotations can occur during an insert only on at most one level

By symmetry, it suffices to examine Cases 2.1 and 2.2. (The ones where the left branch is higher than the right branch by 2.)
In Case 2.2, we can see that after the rebalancing (left rotation, then right rotation), the parent's height will be the same as before the insertion. Hence, the rebalancing algorithm stops here.
In Case 2.1, whether the parent height is one more than before insertion depends on the $\texttt{parent.left.right.height}$. We claim that this value cannot be equal to $\texttt{parent.left.left.height}$. To prove this, we need to make another claim:

**Claim** Suppose there was an insertion operation and we are rebalancing. After a rebalancing operation at a node, if its children's heights are the same, then the node's own height would not have increased relative to before the insertion operation. <br>
**Proof idea** This requires another case analysis and the idea that after an insertion operation and rebalancing, a node's height can either +0 (remain the same) or +1 as compared to before the insertion (proven by induction on the chain of rebalancing operations).

Hence $\texttt{parent.left.right.height} = \texttt{parent.left.left.height} - 1$, so that after rebalancing (right rotation), parent's height remains the same as before rotation. <br/>
This proves the claim. $\square$


We provide derivations for some of the equations in the text.

**Pg. 59** We note that this page is discussing average depth in the context of a "Fibonacci tree", that is a height-balanced tree where each node of height $h$ has 1 subtree of height $h-1$ and the other $h-2$. As mentioned in the book, these results do not hold in general.

1. Eliminating the expression $leaves(h)$

Given recursions
- $depthsum(h) = depthsum(h-1) + depthsum(h-2) + leaves(h)$
- $leaves(h) = leaves(h-1) + leaves(h-2)$

By considering
$depthsum(h) - depthsum(h-1) - depthsum(h-2)$ and expanding each depthsum using the recursion, we will be able to eliminate the $leaves$ term.

2. $\left(\frac{1-\sqrt{5}}{2}\right)^h = o(1)$

Since $1 < \sqrt{5} < 3$, $0 <\frac{\sqrt{5} - 1}{2} < 1$, which proves the result.

3. Average depth

We evaluate the quotient $\frac{depthsum(h)}{leaves(h)}$, where
- $depthsum(h) = \left(\frac{3}{5\sqrt{5}} + \frac{2 + \sqrt{5}}{5} \right)\phi^h + o(1)$
- $leaves(h) = \left(\frac{3+\sqrt{5}}{2\sqrt{5}}\right)\phi^h$
where $\phi = \frac{1 + \sqrt{5}}{2}$

We evaluate that
$$
\begin{equation}
\log_2(leaves(h)) = \log_2(\frac{3+\sqrt{5}}{2\sqrt{5}}) + h\log_2(\phi) + \log_2(o(1)) = h\log_2(\phi) + O(1)
\end{equation}
$$
so that $h = \frac{1}{\log_2(\phi)}(\log_2(leaves(h)) - O(1)) = \frac{1}{\log_2(\phi)}(\log_2(leaves(h))) + O(1)$.

Hence
$$
\begin{equation}
\begin{aligned}
\frac{depthsum(h)}{leaves(h)} &= \frac{\frac{3+5\sqrt{5}}{2\sqrt{5}}+\frac{2+\sqrt{5}}{5}h}{\frac{3+\sqrt{5}}{2\sqrt{5}}}\\ 
&= O(1) + \frac{(2+\sqrt{5}(2\sqrt{5}))}{(3+\sqrt{5})(5)(\log_2(\phi))}\log_2(leaves(h))\\
&\approx O(1) + 1.04\log_2(leaves(h))
\end{aligned}
\end{equation}
$$

Note that the reason why we express the average depth in terms of $\log_2(leaves(h))$ is due to $\lceil \log_2(n) \rceil$ being the height of an optimal search tree with $n$ leaves.
Here, we get a scalar multiple of $1.04$, which is pretty close to $1$. Again, note that this is for the specific case of a "Fibonacci" tree.
