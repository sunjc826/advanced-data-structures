**Pg 38, Point 2**
> The nodes we visit this way are the nodes on the search
path for the beginning of the query interval a, the search path for its end b, and all nodes that are in the tree between these paths. If there are i interior nodes between these paths, there must be at least i + 1 leaves between these paths.

We refer to the "leftmost extreme" search path as L, equivalently the search path of a. We refer to the rightmost extreme search path as R.
Here, we assume that left branch means $<$, right branch means $\geq$.

Note that L and R are not necessarily distinct, in the case where they are the same, then there are no interior nodes between L and R.

**Claim** Suppose a node $v$ is between L and R. First of all this implies L and R are not the same path, and so there must be a node at which L and R diverge. $v$ is characterized by the following. There exists a unique node $u$ on exactly one of L or R where $u$ is an ancestor of $v$, and the child $c$ of $u$ of which $v$ is a descendant diverges from the extreme paths. In other words, $c$ is the first node on the path from tree root to $v$ such that $c$ is not on L or R. Furthermore,
- if $u$ is on L, then $c = u.\texttt{right}$.
- otherwise, $u$ is on R, then $c = u.\texttt{left}$.

**Corollary** Given the above claim, any interior node $v$ between L and R can be traced back to a unique node $c$. (Note that unique here refers to the fact that for a particular $v$, no other node other than $c$ has the same characteristics. It is certainly possible that for distinct $v_1, v_2$, the $c_1, c_2$ that corresponds to each of them can be distinct.) Now consider the set of all such $c_i$, each of them is like the root of a subtree of nodes that are between L and R.
For every such subtree rooted at some $c$, $num\_interior(c) + 1 = num\_leaf(c)$.
Hence, if there are $i > 0$ interior nodes between these paths, and since there is at least one root $c$, there must be at least i + 1 leaves *strictly* between these paths.
Even if there are 0 interior nodes, i.e. L=R, then we must still narrow down to a single node, so we have $i + 1$ leave *nonstrictly* between these paths (where $i = 0$). 


> if this method lists k leaves, the total number of nodes visited is at most twice the number of nodes visited in a normal find operation plus O(k)

Let's dissect this statement. Visit $k$ leaves, in other words $k$ leaves nonstrictly between L and R. Consider the nodes that are visited:
- (1) Nodes on path L
- (2) Nodes on path R
- (3) Interior nodes strictly between L and R
- (4) Leaf nodes strictly between L and R

(1) + (2): twice the number of nodes visited in a normal find op

(4) < (5), where (5) is the leaf nodes nonstrictly between L and R.
Note that (3) < (5) using the corollary above.
Since (5) = k, (3) + (4) = O(k).

Hence time complexity of this method is $2T_{find} + O(k)$
$\square$
