#include "common.h"

typedef int Key;
typedef void *Object;
#define NULL_KEY 0
#define NULL_OBJECT NULL
#define KEY_MAX INT_MAX

typedef struct TreeNode_st
{
    Key key;
    union
    {
        struct TreeNode_st *node;
        Object object;
    } left;
    struct TreeNode_st *right;
    int height;
} TreeNode;

#define LEFT left.node
#define OBJECT left.object
#define RIGHT right
#define Tree_IS_EMPTY(treenode) (((TreeNode *)treenode)->LEFT == NULL)
#define TreeNode_IS_LEAF(treenode) (((TreeNode *)treenode)->RIGHT == NULL)
#define MAX_TREE_DEPTH 200 // It is practically impossible for a balanced BST to have this height or more

typedef struct
{
    TreeNode *arr[MAX_TREE_DEPTH];
    TreeNode **next;
} Stack;

static void Stack_init(Stack *s)
{
    s->next = s->arr;
}

// Does not do overflow checking.
static void Stack_push(Stack *s, TreeNode *n)
{
    *s->next++ = n;
}

static bool Stack_empty(Stack *s)
{
    return s->arr == s->next;
}

// Does not do underflow checking.
static TreeNode *Stack_peek(Stack *s)
{
    return s->next[-1];
}

// Does not do underflow checking.
static TreeNode *Stack_pop(Stack *s)
{
    return *--s->next;
}

static void TreeNode_init(TreeNode *n, Key key)
{
    n->key = key;
    n->LEFT = NULL;
    n->RIGHT = NULL;
    n->height = 0;
}

static TreeNode *NewTreeNode(Key key)
{
    TreeNode *n = malloc(sizeof(TreeNode));
    TreeNode_init(n, key);
    return n;
}

static void TreeNode_free(TreeNode *n)
{
    free(n);
}

static TreeNode *NewTree(void)
{
    return NewTreeNode(NULL_KEY);
}

static void Tree_free(TreeNode *n)
{
    Stack stack;
    Stack_init(&stack);
    Stack_push(&stack, n);
    while (!Stack_empty(&stack))
    {
        TreeNode *node = Stack_pop(&stack);
        if (!TreeNode_IS_LEAF(node))
        {
            Stack_push(&stack, node->LEFT);
            Stack_push(&stack, node->RIGHT);
        }
        TreeNode_free(node);
    }
}

// Precondition: n is an interior node.
static void TreeNode_update_height(TreeNode *n)
{
    n->height = MAX(n->LEFT->height, n->RIGHT->height) + 1;
}

// Precondition: n, n->RIGHT must be interior nodes.
// The child nodes of n, n->RIGHT must have accurate height information.
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

// Precondition: n, n->LEFT must be interior nodes.
// The child nodes of n, n->LEFT must have accurate height information.
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

// Precondition: The left and right subtrees of n should be balanced.
// Furthermore, n->LEFT->height and n->RIGHT->height must have a height difference of 2.
static Tree_rebalance(TreeNode *n)
{
    if (n->LEFT->height == n->RIGHT->height + 2)
    {
        if (n->LEFT->LEFT->height == n->RIGHT->height + 1)
            Tree_right_rotate(n);
        else
        {
            Tree_left_rotate(n->LEFT);
            Tree_right_rotate(n);
        }
    }
    else
    {
        if (n->RIGHT->RIGHT->height == n->LEFT->height + 1)
            Tree_left_rotate(n);
        else
        {
            Tree_right_rotate(n->RIGHT);
            Tree_left_rotate(n);
        }
    }
}

static rebalance_tree_up_ancestor_chain(Stack *ancestor_chain)
{
    TreeNode *ancestor;
    int old_height;
    while (!Stack_empty(ancestor_chain))
    {
        ancestor = Stack_pop(ancestor_chain);
        old_height = ancestor->height;
        if (abs(ancestor->LEFT->height - ancestor->RIGHT->height) == 2)
            Tree_rebalance(ancestor);
        else
            TreeNode_update_height(ancestor);

        if (old_height == ancestor->height)
            break;
    }
}

// pg 55
// Inserts object with rebalancing operations.
// Return Value: Returns true on success, false on failure.
static bool Tree_insert(TreeNode *n, Key key, Object object)
{
    if (Tree_IS_EMPTY(n))
    {
        n->key = key;
        n->OBJECT = object;
        n->RIGHT = NULL;
        return true;
    }

    Stack ancestor_trace, *ancestor_tracep = &ancestor_trace;
    Stack_init(ancestor_tracep);

    // Traversal down tree
    while (!TreeNode_IS_LEAF(n))
    {
        Stack_push(ancestor_tracep, n);
        if (key < n->key)
            n = n->LEFT;
        else
            n = n->RIGHT;
    }

    if (key == n->key)
    {
        fputs("Does not support duplicate keys", stderr);
        return false;
    }

    // Insertion
    TreeNode *old_leaf = NewTreeNode(n->key);
    TreeNode *new_leaf = NewTreeNode(key);
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

    rebalance_tree_up_ancestor_chain(ancestor_tracep);
    return true;
}

// This is not in the book.
// Return Value: Returns the object related to the deleted node, NULL_OBJECT if object not found.
static Object Tree_delete(TreeNode *n, Key key)
{
    Object object;
    if (Tree_IS_EMPTY(n))
        return NULL_OBJECT;

    if (TreeNode_IS_LEAF(n))
    {
        if (n->key != key)
            return NULL_OBJECT;
        object = n->OBJECT;
        n->LEFT = NULL;
        return object;
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

    if (n->key != key)
        return NULL_OBJECT;

    // Deletion
    object = n->OBJECT;
    TreeNode *parent = Stack_pop(ancestor_tracep);
    TreeNode *sibling = (n == parent->LEFT) ? parent->RIGHT : parent->LEFT;

    parent->key = sibling->key;
    parent->left = sibling->left;
    parent->right = sibling->right;
    parent->height = sibling->height;

    TreeNode_free(n);
    TreeNode_free(sibling);

    rebalance_tree_up_ancestor_chain(ancestor_tracep);
    return object;
}
