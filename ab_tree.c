#include "common.h"
typedef int Key;
#define NULL_KEY 0
typedef void *Object;
#define NULL_OBJECT NULL
#define A 4
#define B 8
typedef struct TreeNode_st
{
    int height;
    int degree; // A <= degree <= B
    Key keys[B];
    union
    {
        struct TreeNode_st *nodes[B];
        Object objects[B];
    } children;
} TreeNode;

#define NODES children.nodes
#define OBJECTS children.objects
#define TreeNode_IS_LEAF(treenode) (((TreeNode *)treenode)->height == 0)
#define TreeNode_IS_UNDERFLOWING(treenode) (((TreeNode *)treenode)->degree == (A - 1))
#define TreeNode_IS_ALMOST_UNDERFLOWING(treenode) (((TreeNode *)treenode)->degree == A)
#define TreeNode_IS_FULL(treenode) (((TreeNode *)treenode)->degree == B)
#define Tree_IS_EMPTY(root) (((TreeNode *)root)->degree == 0)

#define MAX_HEIGHT 100 // It is basically impossible for a (a, b)-tree to have this kind of height.

#define STACK_TYPE TreeNode *
#define STACK_MAX MAX_HEIGHT
#include "bounded_stack.h"
#undef STACK_MAX
#undef STACK_TYPE

// Initializes an empty tree node.
static void TreeNode_init(TreeNode *n)
{
    n->height = 0;
    n->degree = 0;
}

static TreeNode *NewTreeNode(void)
{
    TreeNode *n = malloc(sizeof(TreeNode));
    TreeNode_init(n);
    return n;
}

// Does not free its associated object.
static void TreeNode_free(TreeNode *n)
{
    free(n);
}

static TreeNode *NewTree(void)
{
    return NewTreeNode();
}

// Returns index if found, -1 otherwise
static int binary_search_exact(Key *keys, int length, Key query_key)
{
    int low = 0, high = length, mid;
    // Invariant: keys[low ... high) *possibly* contains query_key.
    // More accurately, the arraywise complement keys - keys[low ... high) definitely do not contain query_key.
    while (high - low > 1)
    {
        mid = (low + high) / 2;
        if (query_key < keys[mid])
            high = mid;
        else
            low = mid;
    }

    return (keys[low] == query_key) ? low : -1;
}

// Returns index if found, -1 otherwise
static int binary_search_exact_v2(Key *keys, int length, Key query_key)
{
    int low = 0, high = length - 1, mid;
    // Invariant: keys[low ... high] *possibly* contains query_key.
    // The loop condition can be rewritten as (high - low > 0)
    while (low < high)
    {
        mid = (low + high) / 2;
        if (query_key < keys[mid])
            high = mid - 1;
        else
            low = mid;
    }

    return (keys[low] == query_key) ? low : -1;
}

// This is the most commonly seen form of binary search.
static int binary_search_exact_v3(Key *keys, int length, Key query_key)
{
    int low = 0, high = length - 1, mid;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (query_key < keys[mid])
            high = mid - 1;
        else if (query_key > keys[mid])
            low = mid + 1;
        else
            return mid;
    }

    return -1;
}

// Returns the index of the smallest entry that is greater than the query_key if found,
// otherwise returns length.
static int binary_search_smallest_gt(Key *keys, int length, Key query_key)
{
    int low = 0, mid, high = length - 1;
    while (low < high)
    {
        mid = (low + high) / 2;
        if (query_key < keys[mid])
            high = mid;
        else
            low = mid + 1;
    }

    return (query_key < keys[low]) ? low : length;
}

// Returns the index of the greatest entry that is less than or equal to the query_key if found,
// otherwise returns -1.
// The book actually uses this, but combines this with an exact binary search. Elegant but confusing.
static int binary_search_greatest_leq(Key *keys, int length, Key query_key)
{
    int low = 0, mid, high = length;
    while (high - low > 1)
    {
        mid = (low + high) / 2;
        if (query_key < keys[mid])
            high = mid;
        else
            low = mid;
    }

    return (query_key >= keys[low]) ? low : -1;
}

static Object Tree_find(TreeNode *n, Key key)
{
    int child_index;
    while (!TreeNode_IS_LEAF(n))
    {
        child_index = binary_search_greatest_leq(n->keys[1], n->degree - 1, key) + 1;
        n = n->NODES[child_index];
    }

    child_index = binary_search_exact(n->keys, n->degree, key);
    return (child_index == -1) ? NULL_OBJECT : n->OBJECTS[child_index];
}

// pg 78
// TODO: Handle duplicate keys
static void Tree_insert(TreeNode *n, Key key, Object object)
{
    TreeNode *root = n;
    Stack ancestor_trace, *ancestor_tracep = &ancestor_trace;
    Stack_init(ancestor_tracep);
    int child_index;
    while (!TreeNode_IS_LEAF(n))
    {
        Stack_push(ancestor_tracep, n);
        child_index = binary_search_smallest_gt(n->keys[1], n->degree - 1, key);
        n = n->NODES[child_index];
    }

    if (!TreeNode_IS_FULL(n))
    {
        // Similar to insertion sort
        // Note that the items in a node are left aligned, so we need to start from the right.
        int i;
        for (i = n->degree - 1; i > 0 && key > n->keys[i - 1]; i--)
        {
            n->keys[i] = n->keys[i - 1];
            n->OBJECTS[i] = n->OBJECTS[i - 1];
        }
        n->keys[i] = key;
        n->OBJECTS[i] = object;
        return;
    }

    // Splitting operation
    TreeNode *split_node = NewTreeNode();
    int b_plus_one = B + 1;
    split_node->degree = (B + 1) / 2;
    n->degree = b_plus_one - split_node->degree;
    bool is_object_inserted = false;
    // move stuff over
    for (int i = B - 1, j = split_node->degree - 1; j > 0;)
    {
        if (!is_object_inserted && key < n->keys[i])
        {
            split_node->keys[j] = key, split_node->OBJECTS[j--] = object;
            is_object_inserted = true;
        }
        else
            split_node->keys[j] = n->keys[i], split_node->OBJECTS[j--] = n->OBJECTS[i--];
    }

    if (!is_object_inserted)
    {
        int i;
        for (i = n->degree - 1; i > 0 && key > n->keys[i - 1]; i--)
            n->keys[i] = n->keys[i - 1], n->OBJECTS[i] = n->OBJECTS[i - 1];
        n->keys[i] = key, n->OBJECTS[i] = object;
    }

    // We are done with the leaf nodes, time to maintain the (a, b)-tree structure for the ancestors
    TreeNode *child_node_to_insert = split_node;
    // We are splitting a node associated with some interval [i_1, i_2) to intervals [i_1, i_3), [i_3, i_2)
    // We can let i_3 be the smallest key in the split leaf node.
    Key key_to_insert = child_node_to_insert->keys[0];
    while (!Stack_empty(ancestor_tracep))
    {
        TreeNode *parent = Stack_pop(ancestor_tracep);
        if (!TreeNode_IS_FULL(parent))
        {
            int i;
            for (i = parent->degree - 1; i > 0 && key > parent->keys[i - 1]; i--)
            {
                parent->keys[i] = parent->keys[i - 1];
                parent->NODES[i] = parent->NODES[i - 1];
            }
            parent->keys[i] = key_to_insert;
            parent->NODES[i] = child_node_to_insert;
            return;
        }

        // split operation, this time its an interior node
        split_node = NewTreeNode();
        split_node->height = parent->height;
        split_node->degree = b_plus_one / 2;
        parent->degree = b_plus_one - split_node->degree;
        bool is_node_inserted = false;
        // move stuff over
        for (int i = B - 1, j = split_node->degree - 1; j >= 0;)
        {
            if (!is_node_inserted && key_to_insert < parent->keys[i])
            {
                split_node->keys[j] = key_to_insert, split_node->NODES[j--] = child_node_to_insert;
                is_node_inserted = true;
            }
            else
                split_node->keys[j] = parent->keys[i], split_node->NODES[j--] = parent->NODES[i--];
        }
        if (!is_node_inserted)
        {
            int i;
            // Note the difference in predicate here, i > 1
            // as compared to i > 0 in the code on top.
            for (i = parent->degree - 1; i > 1 && key_to_insert < parent->keys[i - 1]; i--)
                parent->keys[i] = parent->keys[i - 1], parent->NODES[i] = parent->NODES[i - 1];
            parent->keys[i] = key, parent->OBJECTS[i] = object;
        }

        key_to_insert = split_node->keys[0];
        child_node_to_insert = split_node;
    }

    // we need to expand the root
    TreeNode *left = NewTreeNode(), *right = child_node_to_insert;
    left->height = root->height, left->degree = root->degree;
    root->height++, root->degree = 2;
    if (left->height == 0)
    {
        for (int i = 0; i < left->degree; i++)
            left->OBJECTS[i] = root->OBJECTS[i];
    }
    else
    {
        for (int i = 0; i < left->degree; i++)
            left->NODES[i] = root->NODES[i];
    }
    root->keys[1] = key_to_insert;
    root->NODES[0] = left;
    root->NODES[1] = right;
}

#define STACK_TYPE int
#define STACK_TYPE_NAMESPACE int
#define STACK_MAX MAX_HEIGHT
#include "bounded_stack.h"
#undef STACK_MAX
#undef STACK_TYPE_NAMESPACE
#undef STACK_TYPE

static void share_op_rtol(TreeNode *from_right, TreeNode *to_left, TreeNode *parent, int left_index)
{
    TreeNode *left = to_left, *right = from_right;
    int right_index = left_index + 1;
    if (TreeNode_IS_LEAF(left))
    {
        left->keys[left->degree] = right->keys[0], left->OBJECTS[left->degree] = right->OBJECTS[0];
        for (int i = 0; i < right->degree - 1; i++)
            right->keys[i] = right->keys[i + 1], right->OBJECTS[i] = right->OBJECTS[i + 1];
        parent->keys[right_index] = right->keys[0];
    }
    else
    {
        left->keys[left->degree] = parent->keys[right_index];
        parent->keys[right_index] = right->keys[1];
        for (int i = 1; i < right->degree - 1; i++)
            right->keys[i] = right->keys[i + 1], right->NODES[i] = right->NODES[i + 1];
        right->NODES[0] = right->NODES[1];
    }
    left->degree++;
    right->degree--;
}

static void share_op_ltor(TreeNode *from_left, TreeNode *to_right, TreeNode *parent, int right_index)
{
    TreeNode *left = from_left, *right = to_right;
    if (TreeNode_IS_LEAF(right))
    {
        for (int i = 0; i < right->degree; i++)
            right->keys[i + 1] = right->keys[i], right->OBJECTS[i + 1] = right->OBJECTS[i];
        right->keys[0] = left->keys[left->degree - 1];
        right->OBJECTS[0] = left->OBJECTS[left->degree - 1];
        parent->keys[right_index] = right->keys[0];
    }
    else
    {
        for (int i = 1; i < right->degree; i++)
            right->keys[i + 1] = right->keys[i], right->NODES[i + 1] = right->NODES[i];
        right->NODES[1] = right->NODES[0];
        right->keys[1] = parent->keys[right_index];
        parent->keys[right_index] = left->keys[left->degree - 1];
        right->NODES[0] = left->NODES[left->degree - 1];
    }
    left->degree--;
    right->degree++;
}

// joins right to left
static void join_op(TreeNode *left, TreeNode *right, TreeNode *parent, int left_index)
{
    int right_index = left_index + 1;
    if (TreeNode_IS_LEAF(left))
    {
        for (int i = left->degree, j = 0; j < right->degree; i++, j++)
            left->keys[i] = right->keys[j], left->OBJECTS[i] = right->OBJECTS[j];
    }
    else
    {
        for (int i = left->degree + 1, j = 1; j < right->degree; i++, j++)
            left->keys[i] = right->keys[j], left->NODES[i] = right->NODES[j];
        left->keys[left->degree] = parent->keys[right_index];
        left->NODES[left->degree] = right->NODES[0];
    }
    for (int i = right_index; i < parent->degree; i++)
        parent->keys[i] = parent->keys[i + 1], parent->NODES[i] = parent->NODES[i + 1];
    parent->degree--;
    left->degree += right->degree;
    right->degree = 0;
}

// pg 81
static Object Tree_delete(TreeNode *n, Key key)
{
    if (Tree_IS_EMPTY(n))
        return NULL_OBJECT;

    Object object;
    TreeNode *root = n;
    Stack ancestor_trace, *ancestor_tracep = &ancestor_trace;
    Stack_int indices, *indicesp = &indices;
    int child_index;
    while (!TreeNode_IS_LEAF(n))
    {
        Stack_push(ancestor_tracep, n);
        child_index = binary_search_smallest_gt(n->keys[1], n->degree - 1, key);
        Stack_push_int(indicesp, child_index);
        n = n->NODES[child_index];
    }

    child_index = binary_search_exact(n->keys, n->degree, key);
    if (child_index == -1)
        return NULL_OBJECT;

    object = n->OBJECTS[child_index];

    // delete from leaf
    for (int i = child_index; i < n->degree - 1; i++)
        n->OBJECTS[i] = n->OBJECTS[i + 1];
    n->degree--;
    if (!TreeNode_IS_UNDERFLOWING(n))
        return object;

    TreeNode *underflowing_child = n;
    while (!Stack_empty(ancestor_tracep))
    {
        TreeNode *parent = Stack_pop(ancestor_tracep);
        int child_index = Stack_pop_int(indicesp);
        TreeNode *neighbor;
        // last child of parent
        if (child_index == parent->degree - 1)
        {
            neighbor = parent->NODES[child_index - 1];
            // sharing operation
            if (!TreeNode_IS_ALMOST_UNDERFLOWING(neighbor))
            {
                share_op_ltor(neighbor, underflowing_child, parent, child_index);
                return object;
            }
            // joining operation
            else
                join_op(neighbor, underflowing_child, parent, child_index - 1);
        }
        else
        {
            neighbor = parent->NODES[child_index + 1];
            if (!TreeNode_IS_ALMOST_UNDERFLOWING(neighbor))
            {
                share_op_rtol(neighbor, underflowing_child, parent, child_index);
                return object;
            }
            else
                join_op(underflowing_child, neighbor, parent, child_index);
        }

        if (!TreeNode_IS_UNDERFLOWING(parent))
            return object;
        else
            underflowing_child = parent;
    }

    // root node
    if (root->degree >= 2 || TreeNode_IS_LEAF(root))
        return object;

    // root->degree == 1 && root is not a leaf
    TreeNode *only_child = root->NODES[0];
    *root = *only_child;
    TreeNode_free(only_child);

    return object;
}



int main()
{

}
