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

