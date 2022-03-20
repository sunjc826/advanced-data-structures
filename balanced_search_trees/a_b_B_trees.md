**Discussion** A B-tree or an (a,b)-tree both stores a single (large) node in a (possibly external) memory block. In comparison, if a regular binary tree were to be persisted to external memory, a memory block would likely contain many different nodes. Furthermore, it may not be the case that the many different nodes in a memory block are in the vicinity of our search path down a binary tree. Hence, a single search of a binary tree may require fetching many (logarithmic in the best case) memory blocks, for e.g. in the case that each memory block does not contain the immediate descendants of the current node along the search path. Hence, a single large external memory block may only provide the search operation with a single useful node, which is small, causing inefficiency.

In contrast, as a memory block of a B-tree or (a,b)-tree contains 1 (very big) node, having a very large branch factor of at least $a$, a single read from external memory allows us to narrow down the search by a larger factor. (e.g. in a binary search, an interior node reduces the search space by about $\frac{1}{2}$, but a B/(a,b)-tree node reduces the search space by around $\frac{1}{a}$, where $a$ is much larger than 2.) This means that a single fetch from external memory does more work.

**Proposition** For an (a,b)-tree of height $h\geq 1$ where $n$ denotes the number of leaves, $2a^{h-1}\leq n\leq b^h$.
The lower bound on n, $2a^{h-1}\leq n$ derives an upper bound for $h$ given $n$: $h\leq \log_a(n) - \log_a(2) + 1 = \frac{\log n}{\log a} - \log_a(2) + 1$

**Discussion** Binary search
The implementation of (a,b)-tree `find` shows a way of doing binary search that is consistent with the model-1 leaf tree philosophy.
```c++
object_t* model_1_binary_search(int* key_arr, object_t** object_arr, int search_key, int lower_inclusive, int upper_exclusive)
{
    object_t* object;
    int lower;
    int upper;
    int mid;
    lower = lower_inclusive;
    upper = upper_exclusive;
    while (lower + 1 < upper)
    {
        mid = (upper + lower) / 2;
        if (search_key < key_arr[mid])
        {
            upper = mid;
        }
        else // key_arr[mid] <= search_key
        {
            lower = mid;
        }
    }
    
    if (key_arr[lower] == search_key)
    {
        object = object_arr[lower];
    }
    else
    {
        object = NULL;
    }
    return object;
}

object_t* model_2_binary_search((int* key_arr, object_t** object_arr, int search_key, int lower_inclusive, int upper_inclusive)
{
    object_t* object;
    int lower;
    int upper;
    int mid;
    object = NULL;
    while (lower <= upper)
    {
        mid = (upper + lower) / 2;
        if (search_key == key_arr[mid])
        {
            object = object_arr[mid];
            break;
        }
        else if (search_key < key_arr[mid])
        {
            upper = mid - 1;
        }
        else
        {
            lower = mid + 1;
        }
    }
    return object;
}
```
The usual way I would do binary search is via the model 2 way, where in each iteration I divide the search space of $[lower, upper]$ into 3 cases/partitions
$$
[lower, mid - 1], \{mid\}, [mid + 1, upper]
$$
where $lower\leq upper$ (since $upper < lower$ is the termination condition).
- This is guaranteed to terminate since at least one element, mid, is always removed from the search space in each iteration.
- Note that $[lower, upper]$ represents a closed interval.
- An alternative implementation would make $upper <= lower$ the termination condition, and add one more check for equality of keys outside the loop. This makes it more similar to the notion employed by model_1 search tree, that is to terminate when the size of search space $\leq 1$, instead of terminating when size of search space $\leq 0$.

```c++
object_t* model_2_binary_search_alternative((int* key_arr, object_t** object_arr, int search_key, int lower_inclusive, int upper_inclusive)
{
    object_t* object;
    int lower;
    int upper;
    int mid;
    object = NULL;
    while (lower < upper)
    {
        mid = (upper + lower) / 2;
        if (search_key == key_arr[mid])
        {
            object = object_arr[mid];
            break;
        }
        else if (search_key < key_arr[mid])
        {
            upper = mid - 1;
        }
        else
        {
            lower = mid + 1;
        }
    }

    if (search_key == key_arr[lower])
    {
        object = object_arr[lower];
    }
    return object;
}
```

The model 1 way is divides the search space $[lower, upper)$ into 2 cases/partitions
$$
[lower, mid), [mid, upper)
$$
where $lower + 1 < upper$ (since $upper <= lower + 1$ is the termination condition, as $[lower, lower + 1)$ represents a singleton $\{lower\}$).
- This is guaranteed to terminate since $lower + 1 < upper$ implies that $lower< mid <upper$ so $[lower, mid), [mid, upper)\subsetneq [lower, upper)$.


**Proposition** In the long run, each insert/delete can on average cause at most $\frac{3}{2}$ restructuring operations plus possibly one root operation.
Denote
- $T_i$ as a tree after the $i$-th insertion/deletion (plus any additional restructuring) at the lowest level, in particular, $T_0$ is the initial tree.
- $V_i$ as the set of vertices of $T_i$
- $n$ as a node
- $\phi$ as the potential function. $\phi(n)$ is the potential of a tree node $n$. $\phi(T_i)$ is the potential of the tree after the $i$-th insertion/deletion. Note that $\phi(T_i) = \sum_{n\in V_i}\phi(n)$
- $M_N$ as the average number of restructuring operations caused by $N$ insertions/deletions. 
We use the following lemmas:
- $0\leq \phi(T_0)\leq 6|V_0|$
- Each insertion/deletion causes at the lowest level increases the tree potential by at most $3$.
- Each restructuring operation decreases the tree potential by at least $2$.

By definition of $\phi(n)$, at any step, $\forall n\in V_i, \phi(n)\geq 0$ so that $\phi(T_i)\geq 0$. Also, for any $i$, $\Delta \phi(T_i) \leq 3 - 2\cdot \text{m_i}$ where $m_i$ is the number of restructuring operations caused by the ith insertion/deletion. Note that $M_N = \frac{1}{N}\sum_{1\leq i\leq N}m_i$.

Hence, the following inequality holds for all $N\in \mathbb{Z}^+$:
$$
    6|V_0| + (3N - 2M_NN) \geq \phi(T_0) + (\phi(T_n) - \phi(T_0)) = \phi(T_n) \geq 0
$$
Hence, $M_N\leq \frac{3|V_0|}{N} + \frac{3}{2}$, and taking limits gives $\lim_{N\rightarrow\infty}M_N\leq \frac{3}{2}$. $\square$

**Discussion** Insertion in a non-leaf node of an (a,b)-tree
Let $n$ be a non-leaf node with search space $[A, B]$ for some $A, B$. Let $key$ be the `key` array of $n$. This array partitions the search space $[A, B]$ into
$$
[A, key[1]), [key[1], key[2]), \dots [key[key.length - 1], B)
$$
Important! Note that we have no guarantee that $key[0] = A$. So in other words, if we are only given node $n$ and its descendants, we would not know what $A$ actually is.

Consider what happens if we need to insert a child node $c$ under $n$, given that the search space of $c$ is $[j, k)\subseteq [A, B)$ and that we are guaranteed that $[j, k)$ either
- Lies between the child nodes $[k_1, k_2), [k_2, k_3)$ such that $k \leq k_2$
  In this case, we simply can do this: $[k_1, j), [j, k_2), [k_2, k_3)$
- Lies after the last child node, i.e. $[k_1, k_2)$ such that $k\leq k_2$
  In this case, we can do this: $[k_1, j), [j, k)$.
In either case, we are partitioning the search space $[k_1, k_2)$ into $[k_1, j), [j, k_2)$. In code terms, we only need to change 1 key value in the `key` array, i.e. set it to $j$, which is known.

*In other words*, if we can guarantee that $[j, k)$ is in the upper end of some $[k_1, k_2)$, we can easily insert $[j, k)$ by
- inserting $j$ into the `key` array *before* $k_2$ and,
- inserting the node representing $[j, k_2)$ *after* the node representing $[k_1, j)$. In particular, $[j, k_2)$ will not be the first node.
