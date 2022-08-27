// pg 148
// using height balanced tree as the underlying BBST
#include "common.h"
#include "element_types.h"

// represents a closed interval [a, b]
typedef struct Interval_st
{
    Endpoint a;
    Endpoint b;
} Interval;

static bool Interval_contains(Interval *interval, int number)
{
    return interval->a <= number && number <= interval->b;
}

typedef struct ListNode_st
{
    struct ListNode_st *next;
    Endpoint endpoint;
    Interval *interval;
} ListNode;

static ListNode *NewListNode(Endpoint endpoint, Interval *interval)
{
    ListNode *n = malloc(sizeof(ListNode));
    n->next = NULL;
    n->endpoint = endpoint;
    n->interval = interval;
    return n;
}

typedef struct TreeNode_st
{
    Key key;
    union
    {
        struct TreeNode_st *node;
        Endpoint *endpoint;
    } left;
    struct TreeNode_st *right;
    int height;
    ListNode *left_list;
    ListNode *right_list;
} TreeNode;

#define LEFT left.node
#define ENDPOINT left.endpoint
#define RIGHT right
#define Tree_IS_EMPTY(treenode) (((TreeNode *)treenode)->LEFT == NULL)
#define TreeNode_IS_LEAF(treenode) (((TreeNode *)treenode)->RIGHT == NULL)

#define STACK_TYPE TreeNode *
#define STACK_MAX 200
#include "bounded_stack.h"
#undef STACK_MAX
#undef STACK_TYPE

static void TreeNode_init(TreeNode *n, Key key)
{
    n->key = key;
    n->LEFT = NULL;
    n->RIGHT = NULL;
    n->height = 0;
    n->left_list = NULL;
    n->right_list = NULL;
}

static TreeNode *NewTreeNode(Key key)
{
    TreeNode *n = malloc(sizeof(TreeNode));
    TreeNode_init(n, key);
    return n;
}

static TreeNode *NewTreeNode_leaf(Key key, Endpoint *endpoint)
{
    TreeNode *n = NewTreeNode(key);
    n->ENDPOINT = endpoint;
    return n;
}

static TreeNode *NewTreeNode_root(void)
{
    return NewTreeNode(NULL_KEY);
}

static TreeNode_write_leaf(TreeNode *n, Key key, Endpoint *endpoint)
{
    n->key = key;
    n->ENDPOINT = endpoint;
}

static void TreeNode_update_height(TreeNode *n)
{
    n->height = MAX(n->LEFT->height, n->RIGHT->height);
}

static void Tree_left_rotate(TreeNode *n)
{
    TreeNode *right = n->RIGHT;
    n->RIGHT = right->RIGHT;
    right->RIGHT = right->LEFT;
    right->LEFT = n->LEFT;
    n->LEFT = right;
    SWAP(Key, n->key, right->key);
    TreeNode_update_height(right);
    TreeNode_update_height(n);
}

static void Tree_right_rotate(TreeNode *n)
{
    TreeNode *left = n->LEFT;
    n->LEFT = left->LEFT;
    left->LEFT = left->RIGHT;
    left->RIGHT = n->RIGHT;
    n->RIGHT = left;
    SWAP(Key, n->key, left->key);
    TreeNode_update_height(left);
    TreeNode_update_height(n);
}

// Precondition: n is unbalanced, i.e. abs(n->LEFT->height - n->RIGHT->height) >= 2
// Additionally, all of n's child nodes must be balanced.
static void Tree_rebalance(TreeNode *n)
{
    if (n->LEFT->height > n->RIGHT->height)
    {
        if (n->LEFT->LEFT->height + 1 == n->LEFT->height)
            Tree_right_rotate(n);
        else
        {
            Tree_left_rotate(n->LEFT);
            Tree_right_rotate(n);
        }
    }
    else
    {
        if (n->RIGHT->RIGHT->height + 1 == n->RIGHT->height)
            Tree_left_rotate(n);
        else
        {
            Tree_right_rotate(n->RIGHT);
            Tree_left_rotate(n);
        }
    }
}

// No duplicate keys, but this doesn't really matter.
static void Tree_insert_endpoint(TreeNode *n, Endpoint *endpoint)
{
    Key key = *endpoint;
    if (Tree_IS_EMPTY(n))
    {
        TreeNode_write_leaf(n, key, endpoint);
        return;
    }

    Stack ancestor_trace, *ancestor_tracep = &ancestor_trace;
    Stack_init(ancestor_tracep);
    while (!TreeNode_IS_LEAF(n))
    {
        Stack_push(ancestor_tracep, n);
        if (key < n->key)
            n = n->LEFT;
        else
            n = n->RIGHT;
    }

    if (n->key == key)
        return;

    // insert
    TreeNode *old_leaf = NewTreeNode_leaf(n->key, n->ENDPOINT);
    TreeNode *new_leaf = NewTreeNode_leaf(key, endpoint);
    if (key < n->key)
    {
        n->LEFT = new_leaf;
        n->RIGHT = old_leaf;
    }
    else
    {
        n->key = key;
        n->LEFT = old_leaf;
        n->RIGHT = new_leaf;
    }

    n->height = 1;

    // rebalance starting from n's parent
    while (!Stack_empty(ancestor_tracep))
    {
        TreeNode *ancestor = Stack_pop(ancestor_tracep);
        int old_height = ancestor->height;
        if (abs(ancestor->LEFT->height - ancestor->RIGHT->height) == 2)
            Tree_rebalance(ancestor);
        if (old_height == ancestor->height)
            break;
    }
}

typedef int (*Comparator)(Interval, Interval);

static void merge(Interval *intervals[], Comparator comp, int left, int mid, int right)
{
    Interval *merged_arr[right - left + 1];
    int i, j, base;
    for (base = 0, i = left, j = mid + 1; i <= mid && j <= right;)
    {
        if (comp(*intervals[i], *intervals[j]) < 0)
            merged_arr[base++] = intervals[i++];
        else
            merged_arr[base++] = intervals[j++];
    }

    while (i <= mid)
        merged_arr[base++] = intervals[i++];

    while (j <= right)
        merged_arr[base++] = intervals[j++];

    for (i = left, base = 0; i <= right;)
        intervals[i++] = merged_arr[base++];
}

// mergesort
// Params:
// - left: inclusive
// - right: inclusive
static void sort_intervals(Interval *intervals[], Comparator comp, int left, int right)
{
    if (right <= left)
        return;

    int mid = (left + right) / 2;
    sort_intervals(intervals, comp, left, mid);
    sort_intervals(intervals, comp, mid + 1, right);
    merge(intervals, comp, left, mid, right);
}

static int Interval_left_endpoint_desc(Interval i1, Interval i2)
{
    if (i1.a < i2.a)
        return 1;
    else if (i1.a > i2.a)
        return -1;
    else
        return 0;
}

static int Interval_right_endpoint_asc(Interval i1, Interval i2)
{
    if (i1.b < i2.b)
        return -1;
    else if (i1.b > i2.b)
        return 1;
    else
        return 0;
}

// Precondition: The necessary setup (endpoint insertion) must have been done. Hence, the
// TreeNode is guaranteed to be found.
static TreeNode *find_interior_node_for_interval(TreeNode *root, Interval *interval)
{
    TreeNode *current = root;
    while (true)
    {
        if (Interval_contains(interval, current->key))
            return current;
        else if (interval->b < current->key)
            current = current->LEFT;
        else
            current = current->RIGHT;
    }
}

static void insert_interval_left(TreeNode *root, Interval *interval)
{
    TreeNode *interior_node = find_interior_node_for_interval(root, interval);

    ListNode *list_node = NewListNode(interval->a, interval);
    list_node->next = interior_node->left_list;
    interior_node->left_list = list_node;
}

static void insert_interval_right(TreeNode *root, Interval *interval)
{
    TreeNode *interior_node = find_interior_node_for_interval(root, interval);

    ListNode *list_node = NewListNode(interval->b, interval);
    list_node->next = interior_node->right_list;
    interior_node->right_list = list_node;
}

static TreeNode *make_tree(Interval *intervals[], int intervals_len)
{
    TreeNode *tree = NewTreeNode_root();
    for (int i = 0; i < intervals_len; i++)
    {
        Tree_insert_endpoint(tree, &intervals[i]->a);
        Tree_insert_endpoint(tree, &intervals[i]->b);
    }

    // sort by left endpoint, descending
    sort_intervals(intervals, Interval_left_endpoint_desc, 0, intervals_len - 1);
    for (int i = 0; i < intervals_len; i++)
        insert_interval_left(tree, intervals[i]);

    // sort by right endpoint, ascending
    sort_intervals(intervals, Interval_right_endpoint_asc, 0, intervals_len - 1);
    for (int i = 0; i < intervals_len; i++)
        insert_interval_right(tree, intervals[i]);
    return tree;
}

int main()
{
    return 0;
}
