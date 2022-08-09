#include "common.h"
typedef int Key;
#define NULL_KEY 0
#define KEY_MAX INT_MAX
typedef void *Object;
#define NULL_OBJECT NULL

typedef enum
{
    Color_RED,
    Color_BLACK,
} Color;

typedef struct TreeNode_st
{
    Key key;
    Color color;
    union
    {
        struct TreeNode_st *node;
        Object object;
    } left;
    struct TreeNode_st *right;
} TreeNode;

#define LEFT left.node
#define OBJECT left.object
#define RIGHT right
#define Tree_IS_EMPTY(treenode) (((TreeNode *)treenode)->OBJECT == NULL_OBJECT)
#define TreeNode_IS_LEAF(treenode) (((TreeNode *)treenode)->RIGHT == NULL)

static void TreeNode_init(TreeNode *n, Key key, Color color)
{
    n->key = key;
    n->color = color;
    n->OBJECT = NULL;
    n->RIGHT = NULL;
}

static TreeNode *NewTreeNode(Key key, Color color)
{
    TreeNode *n = malloc(sizeof(TreeNode));
    TreeNode_init(n, key, color);
    return n;
}

static TreeNode *NewTreeNodeLeaf(Key key, Object object)
{
    TreeNode *n = NewTreeNode(key, Color_RED);
    n->OBJECT = object;
    return n;
}

static TreeNode *NewTree(void)
{
    TreeNode *tree = NewTreeNode(NULL_KEY, Color_BLACK); // Root is always black.
    return tree;
}

static Object Tree_find(TreeNode *n, Key key)
{
    if (Tree_IS_EMPTY(n))
        return NULL_OBJECT;
    while (!TreeNode_IS_LEAF(n))
    {
        if (key < n->key)
            n = n->LEFT;
        else
            n = n->RIGHT;
    }

    return (key == n->key) ? n->OBJECT : NULL_OBJECT;
}

#define STACK_TYPE TreeNode *
#define STACK_MAX 300
#include "bounded_stack.h"
#undef STACK_MAX
#undef STACK_TYPE

static void Tree_left_rotate(TreeNode *n)
{
    TreeNode *right = n->RIGHT;
    n->RIGHT = right->RIGHT;
    right->RIGHT = right->LEFT;
    right->LEFT = n->LEFT;
    n->LEFT = right;
    SWAP(Key, n->key, right->key);
}

static void Tree_right_rotate(TreeNode *n)
{
    TreeNode *left = n->LEFT;
    n->LEFT = left->LEFT;
    left->LEFT = left->RIGHT;
    left->RIGHT = n->RIGHT;
    n->RIGHT = left;
    SWAP(Key, n->key, left->key);
}

// Params:
// - `current` refers to the node at which the red-black condition is violated
// - `parent` is the parent node of current
// Precondition: current's color is red, and so are its children
static void insert_rebalance_special_case(TreeNode *current, TreeNode *parent)
{
    TreeNode *sibling;
    if (current == parent->LEFT)
    {
        sibling = parent->RIGHT;
        if (sibling->color == Color_RED)
            goto case_1;

        assert(false); // The book didn't mention this, but I believe cases 2 and 3 cannot occur at the insertion point.
    }
    else
    {
        sibling = parent->LEFT;
        if (sibling->color == Color_RED)
            goto case_1;

        assert(false);
    }

case_1:
    current->color = Color_BLACK;
    sibling->color = Color_BLACK;
    parent->color = Color_RED;
    return;
}

static void insert_rebalance(TreeNode *current, TreeNode *parent)
{
    TreeNode *sibling;
    if (current == parent->LEFT)
    {
        sibling = parent->RIGHT;
        if (sibling->color == Color_RED)
            goto case_1;
        else
        {
            if (current->RIGHT->color == Color_BLACK)
                goto case_2_1;
            else
                goto case_2_2;
        }
    }
    else
    {
        sibling = parent->LEFT;
        if (sibling->color == Color_RED)
            goto case_1;
        else
        {
            if (current->LEFT->color == Color_BLACK)
                goto case_3_1;
            else
                goto case_3_2;
        }
    }

case_1:
    current->color = Color_BLACK;
    sibling->color = Color_RED;
    parent->color = Color_RED;
    return;
case_2_1:
    Tree_right_rotate(parent);
    return;
case_2_2:
    Tree_left_rotate(current);
    Tree_right_rotate(parent);
    return;
case_3_1:
    Tree_left_rotate(parent);
    return;
case_3_2:
    Tree_right_rotate(current);
    Tree_left_rotate(parent);
    return;
}

// pg 98
// Note that the book's implementation does not take into account the special case
// so the book's implementation does not have a worst case O(1) rotations guarantee.
static bool Tree_insert(TreeNode *n, Key key, Object object)
{
    if (Tree_IS_EMPTY(n))
    {
        n->key = key;
        n->OBJECT = object;
        return true;
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

    if (key == n->key)
        return false;

    // insertion
    TreeNode *old_leaf = NewTreeNode(n->key, Color_RED);
    TreeNode *new_leaf = NewTreeNode(key, Color_RED);
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

#define IS_ROOT() (Stack_empty(ancestor_tracep))
    TreeNode *current = n;

    if (current->color == Color_BLACK || IS_ROOT()) // since we maintain the invariant that root->color == Color_BLACK
        return true;

    // special case: 1st rebalance
    // by considering this special case, we can attain a worst case no. of 3 rotations
    TreeNode *parent;
#define UPDATE_CURRENT() current = Stack_pop(ancestor_tracep)
#define UPDATE_PARENT() parent = Stack_pop(ancestor_tracep)
    UPDATE_PARENT();
    insert_rebalance_special_case(current, parent);

    // after first rebalance
    while (parent->color == Color_RED)
    {
        if (IS_ROOT())
        {
            parent->color = Color_BLACK;
            break;
        }

        UPDATE_CURRENT();
        if (current->color == Color_BLACK)
            break;
        else if (IS_ROOT())
        {
            current->color = Color_BLACK;
            break;
        }

        UPDATE_PARENT();
        insert_rebalance(current, parent);
    }
    return true;
#undef UPDATE_PARENT
#undef UPDATE_CURRENT
#undef IS_ROOT
}

// Prints all leaf nodes.
static void Tree_inorder_print(const TreeNode *node)
{
    if (TreeNode_IS_LEAF(node))
        printf("%d ", node->key);
    else
    {
        Tree_inorder_print(node->LEFT);
        Tree_inorder_print(node->RIGHT);
    }
}

static TreeNode *Tree_make_list(TreeNode *n)
{
    if (Tree_IS_EMPTY(n))
        return NULL;

    Stack stack, *stackp = &stack;
    Stack_init(stackp);
    TreeNode *linked_list = NULL;
    Stack_push(stackp, n);
    while (!Stack_empty(stackp))
    {
        TreeNode *node = Stack_pop(stackp);
        if (TreeNode_IS_LEAF(node))
        {
            TreeNode *linked_list_node = NewTreeNodeLeaf(node->key, node->OBJECT);
            linked_list_node->RIGHT = linked_list;
            linked_list = linked_list_node;
        }
        else
        {
            Stack_push(stackp, node->LEFT);
            Stack_push(stackp, node->RIGHT);
        }
    }

    return linked_list;
}

typedef TreeNode Tree;
typedef struct
{
    Key key;
    int insertion_seq_num;
} TestObject;

static TestObject *NewTestObject(Key key, int seq_num)
{
    TestObject *o = malloc(sizeof(TestObject));
    o->key = key;
    o->insertion_seq_num = seq_num;
    return o;
}

int main()
{
    Tree *tree = NewTree();
    assert(Tree_find(tree, 1) == NULL_OBJECT);

#define NUM_TRIALS 1000

    int r;
    Key keys[NUM_TRIALS];
    puts("Testing Tree_insert");
    for (int i = 0; i < NUM_TRIALS; i++)
    {
        do
        {
            r = random() % KEY_MAX;
        } while (Tree_find(tree, r) != NULL_OBJECT);
        keys[i] = r;
        Tree_insert(tree, r, NewTestObject(r, i));
    }

    Tree_inorder_print(tree);
    TreeNode *linked_list = Tree_make_list(tree);
    TreeNode *current = linked_list;
    assert(current != NULL);
    while (current->RIGHT != NULL)
    {
        assert(current->key < current->RIGHT->key);
        current = current->RIGHT;
    }
#undef NUM_TRIALS
    puts("Tests passed");
    return 0;
}
