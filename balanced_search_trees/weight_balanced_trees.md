**Discussion** When establishing bounds between $n, h$ for $\alpha$ weight balanced trees (where $\alpha\in (0, \frac{1}{2})$), the idea is to show that $(1-\alpha) leaves(T_h)\geq leaves(T_{h-1})$.

We obtain $h\leq \frac{1}{\log_2(\frac{1}{1-\alpha})}\log_2(n)$. We observe that as $\alpha\rightarrow 0.5$, i.e. the tree becomes more balanced, $\frac{1}{\log_2(\frac{1}{1-\alpha})}\rightarrow 1$, whereas if $\alpha\rightarrow 0$, then $\frac{1}{\log_2(\frac{1}{1-\alpha})}\rightarrow \infty$.

**Discussion** These inequalities are equivalent.
- $\alpha node.wt\leq left(node).wt\leq (1-\alpha) node.wt$
- $\alpha right(node).wt = \alpha (node.wt - left(node).wt)\leq (1-\alpha) left(node).wt$
- $\alpha left(node).wt \leq (1-\alpha) right(node).wt = (1-\alpha) (node.wt - left(node).wt)$

**Pg 64**
We discuss the bounds of $\delta$.
Let $x$ be the weight of $n^{old}\rightarrow right\rightarrow weight$ in case 2. $w, \alpha$ are as defined in the book. Then we have
- $x < \alpha w$ as in case 2.
- $x \geq \alpha(w-1)\lor x-1\geq \alpha(w-1) \lor x\geq \alpha(w+1)\lor x+1\geq \alpha(w+1)$
  - The first is when a node was inserted in the left subtree.
  - The second is when a node was inserted in the right subtree.
  - The third is when a node was deleted from the left subtree. (i.e previously, there were $w+1$ leaves in left and right subtree combined, and the right subtree had $x$ leaves)
  - The fourth is when a node was deleted from the right subtree. (i.e. the right subtree previously had $x+1$ leaves)

The second inequality resolves to $x\geq \alpha(w-1)\lor x+1\leq \alpha(w+1)$ since the other 2 cases will contradict $x < \alpha w$. This then implies $x\geq \alpha w - \alpha\lor x\geq \alpha w - (1-\alpha)$.
Combining the two, we get $\alpha w - \Delta \geq x < \alpha w$, where $\Delta\in \{\alpha, 1-\alpha\}$. Hence, $x = \lfloor\alpha w\rfloor = \alpha w - \delta$ for some $\delta\in (0, 1]$.

**Pg 67** 
In case 2.1's condition(a),
$n^{new}\rightarrow right\rightarrow right\rightarrow weight = \lfloor\alpha w\rfloor$ as discussed above.

$n^{new}\rightarrow right\rightarrow weight = n^{new}\rightarrow weight - n^{new}\rightarrow left\rightarrow weight\leq w - \lceil \alpha w\rceil$ since $n^{new}\rightarrow left\rightarrow weight \geq \alpha w$.

Hence $ \lfloor\alpha w\rfloor > \alpha(w - \lceil \alpha w\rceil)$ is a sufficient condition for condition (a) to hold.

While not mentioned here in the book, I believe that condition (c) also needs to be checked.
More precisely, we need to show that $\alpha w\geq \delta$ holds even when $\alpha^2 w < 1$. Using the result in the previous discussion $\delta\in\{\alpha, 1-\alpha\}$.
- When $\delta = \alpha$, the inequality trivially holds as $w\geq 1$.
- When $\delta = 1-\alpha$, the inequality is equivalent to $w\geq \frac{1}{\alpha}-1$

Since $\alpha\in (\frac{2}{7}, 1-\frac{1}{\sqrt{2}})$, we see that $\frac{1}{\alpha}-1\geq \frac{7}{2} - 1 = 2.5$, i.e. $w\geq 3$. But this is true, since $w\geq 2$ as it has both left and right subtrees, furthermore if $w = 2$, then there will be no balancing issues, so $w \geq 3$ as desired.

TODO: Case 2.2 (I'm lazy)

**Pg 70-71** We give an alternative derivation for the coefficient $c = \frac{-1}{\alpha\log\alpha + (1-\alpha)\log(1-\alpha)}$.
At this point, we pretend that we don't have any idea of what $c$ should be for the average depth bound.
We know that the worst case height bound $O(\log n)$, so the average depth bound should not be worse than this. Hence, assuming that worst case $\text{averagedepth}(n) \leq c\log n$ for some $c$, we equivalently have worst case depth sum $D_n\leq cn\log n$.

We note that
$$
D_n = n + \max \{D_a + D_b : a+b = n\land a,b\geq \alpha n\}
$$

We assume that for some $c$, $\forall k<n, D_k\leq ck\log k$. To show that $D_n\leq cn\log n$, it suffices to show for all $a, b$ satisfying $a+b = n\land a,b\geq \alpha n$, $n + D_a + D_b\leq n + ca\log a + cb\log b\leq cn\log n$.

$$
\begin{equation}
\begin{aligned}
n + ca\log a + cb\log b&\leq cn\log n\iff\\
n&\leq c(a\log n - a\log a + b\log n - b\log b)\iff\\
c^{-1}&\leq \frac{a}{n}\log\frac{n}{a} + \frac{b}{n}\log\frac{n}{b}\iff\\
-c^{-1}&\geq \frac{a}{n}\log\frac{a}{n} + \frac{b}{n}\log\frac{b}{n}
\end{aligned}
\end{equation}
$$

Since $\frac{a}{n}+\frac{b}{n} = 1$ and $\frac{a}{n}, \frac{b}{n}\geq \alpha$, and $x\mapsto x\log x + (1-x)\log x$ is decreasing on $[0,\frac{1}{2}]$, it suffices for $-c^{-1}\geq \alpha\log\alpha + (1-\alpha)\log(1-\alpha)$, equivalently $c\geq \frac{-1}{\alpha\log\alpha + (1-\alpha)\log(1-\alpha)}$. We set $c = \frac{-1}{\alpha\log\alpha + (1-\alpha)\log(1-\alpha)}$ since this value is positive.

**Pg 71**
**Theorem**. In the time from one rebalancing of a specific node to the next rebalancing of this node, a positive fraction of all leaves below that node are changed.

From my basic understanding, the proof of this theorem suggests that the rebalancing operations keep the tree $\alpha^*$ balanced, where $\alpha^* > \alpha$ due to the $\epsilon$ parameter. Hence for a node to go from $\alpha^*$ balanced to $\alpha$-imbalanced, a positive fraction $frac$ (for e.g. say around $\alpha^*-\alpha$ (this is a completely madeup value I'm using to illustrate my point, so it may be completely wrong)) of leaves needs to be inserted/deleted.
This means for comparison nodes higher up in the tree with large $weight$, many more leaves $frac\cdot weight$ need to be changed before such nodes need rebalancing.
