**Pg 286** Here, I elaborate on the summation for bounding the work done on group 1 nodes across $m$ operations.
First, we consider splitting the work by level. All the possible level values reachable by any node is bounded below by $\alpha(n)$.
$$
\begin{equation}
    \sum_{\text{levels} i\in \{0, 1, 2, \dots, \alpha(n)\} } \sum_{\text{block indices} j\in \mathbb{N} } work_{i, j}
\end{equation}
$$ 
where $work_{i, j}$ is the amount of work done on any nodes during the time (over all $m$ operations) in which they were of $level = i$ and resided in $B_{i, j}$.

It turns out that we can bound $work_{i, j}$ by the number of nodes that were *at some point* satisfied this criterion of having $level = i\land rank\in B_{i, j}$ multiplied by the upper bound on the work done on each such node.
The former is $n_{i, j}$ and the latter upper bound can be $b_{i, j} - 1$ since each time a node is involved in a path compression, for its level to remain unchanged, its `->up->rank` will increase by at least one. After this happens $b_{i, j}$ or more times, its level must increase.


Let's also examine the actual computation of the summation. We can derive that
$$
\begin{equation}
    \sum_{k\geq l}k\lambda^k = \frac{\lambda^{l+1}}{(1-\lambda)^2} + \frac{l\lambda^{l+1}}{1-\lambda}
\end{equation}
$$
for $|\lambda| < 1$. Now this isn't so complicated when $\lambda = \frac{1}{2}$, since $1-\lambda = \lambda$, so many things cancel out. So just apply this a few times, and we will obtain the final bound of $O(n\alpha(n))$.

Some intuition on why the upper bound on the total amount of work done on group 1 nodes is independent of $m$. Consider a case where after no more joins take place, and path compression has been done on all the nodes. Then the path length from any non root node to the root is just 1.

We can make our intuition even more precise. 
1. Firstly $\alpha(n)$ is a small number, almost $O(1)$. Hence, a node can only "level-up" (i.e. level increase) so many times.
2. Furthermore, within each level, the higher $j$ gets, the higher the rank gets, but as we know, there are fewer and fewer nodes of high rank, i.e. $n_{i, j}$ shrinks as $j$ increases.
3. Lastly, a node can only stay so long in its current level (this is the $b_{i, j} - 1$ bound) if it wants to keep contributing to the work done.
4. We should also note that the product $n_{i, j}(b_{i, j} - 1)$ shrinks since even though $b_{i, j}$ may increase, $n_{i, j}$ decreases exponentially w.r.t. the rate of change of $b_{i, j}$

All these factors cause the resulting bound to be good.

**Remark** Each block in $B_i$ is neatly partitioned by blocks in $B_{i-1}$. This "neatness" property is nice, but not crucial for the proof. We only really want to know that each block in $B_i$ overlaps with a certain number $b_{i, j}$ of blocks in $B_{i - 1}$. This $b_{i, j}$ is then used later in the summation.
