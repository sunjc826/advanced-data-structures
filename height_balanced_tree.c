#include "common.h"
#include "bbst.h"
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

static TreeNode *NewTreeNodeLeaf(Key key, Object object)
{
    TreeNode *n = NewTreeNode(key);
    n->OBJECT = object;
    return n;
}

void TreeNode_free(TreeNode *n)
{
    free(n);
}

TreeNode *NewTree(void)
{
    return NewTreeNode(NULL_KEY);
}

void Tree_free(TreeNode *n)
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

Object Tree_find(TreeNode *n, Key key)
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

    return (n->key == key) ? n->OBJECT : NULL_OBJECT;
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
static void Tree_rebalance(TreeNode *n)
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

static void rebalance_tree_up_ancestor_chain(Stack *ancestor_chain)
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
bool Tree_insert(TreeNode *n, Key key, Object object)
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
    TreeNode *old_leaf = NewTreeNodeLeaf(n->key, n->OBJECT);
    TreeNode *new_leaf = NewTreeNodeLeaf(key, object);
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
Object Tree_delete(TreeNode *n, Key key)
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

// Precondition: n must be the root of an non-empty tree.
Key Tree_min(TreeNode *n)
{
    while (!TreeNode_IS_LEAF(n))
        n = n->LEFT;

    return n->key;
}

// Return Value: Returns the root of the tree resulting from the join operation.
TreeNode *join_trees(TreeNode *smaller, TreeNode *larger)
{
    if (abs(smaller->height - larger->height) <= 1)
        goto case_1;
    else if (smaller->height < larger->height)
        goto case_2;
    else
        goto case_3;

case_1:
{
    TreeNode *new_root = NewTree();
    new_root->LEFT = smaller;
    new_root->RIGHT = larger;
    new_root->key = Tree_min(larger);
    new_root->height = MAX(smaller->height, larger->height) + 1;
    return new_root;
}

case_2:
{
    Stack ancestor_trace, *ancestor_tracep = &ancestor_trace;
    Stack_init(ancestor_tracep);
    TreeNode *current = larger;
    while (current->height > smaller->height)
    {
        Stack_push(ancestor_tracep, current);
        current = current->LEFT;
    }
    TreeNode *parent = Stack_peek(ancestor_tracep);
    TreeNode *new_subroot = NewTreeNode(Tree_min(current));
    new_subroot->LEFT = smaller;
    new_subroot->RIGHT = larger;
    parent->LEFT = new_subroot;
    new_subroot->height = smaller->height + 1;
    // Technically, this entire if-else can be reduced to a single call of rebalance_tree_up_ancestor_chain(ancestor_tracep)
    // But we will be more explicit.
    if (current->height == smaller->height)
    {
        // Case 2.1
        if (parent->height == current->height + 2)
            ;
        // Case 2.2
        else
            rebalance_tree_up_ancestor_chain(ancestor_tracep);
    }
    // Case 2.3
    else
        rebalance_tree_up_ancestor_chain(ancestor_tracep);
    return larger;
}

case_3:
{
    Stack ancestor_trace, *ancestor_tracep = &ancestor_trace;
    Stack_init(ancestor_tracep);
    TreeNode *current = smaller;
    while (current->height > larger->height)
    {
        Stack_push(ancestor_tracep, current);
        current = current->RIGHT;
    }

    TreeNode *parent = Stack_peek(ancestor_tracep);
    TreeNode *new_subroot = NewTreeNode(Tree_min(larger));
    new_subroot->LEFT = current;
    new_subroot->RIGHT = larger;
    parent->RIGHT = new_subroot;
    new_subroot->height = larger->height + 1;

    if (current->height == larger->height)
    {
        if (parent->height == current->height + 2)
            ;
        else
            rebalance_tree_up_ancestor_chain(ancestor_tracep);
    }
    else
        rebalance_tree_up_ancestor_chain(ancestor_tracep);
    return smaller;
}
}

TreeNode *join_trees_with_known_key(TreeNode *smaller, TreeNode *larger, Key key)
{
    if (abs(smaller->height - larger->height) <= 1)
        goto case_1;
    else if (smaller->height < larger->height)
        goto case_2;
    else
        goto case_3;

case_1:
{
    TreeNode *new_root = NewTree();
    new_root->LEFT = smaller;
    new_root->RIGHT = larger;
    new_root->key = key;
    new_root->height = MAX(smaller->height, larger->height) + 1;
    return new_root;
}

case_2:
{
    Stack ancestor_trace, *ancestor_tracep = &ancestor_trace;
    Stack_init(ancestor_tracep);
    TreeNode *current = larger;
    while (current->height > smaller->height)
    {
        Stack_push(ancestor_tracep, current);
        current = current->LEFT;
    }
    TreeNode *parent = Stack_peek(ancestor_tracep);
    TreeNode *new_subroot = NewTreeNode(key);
    new_subroot->LEFT = smaller;
    new_subroot->RIGHT = larger;
    parent->LEFT = new_subroot;
    new_subroot->height = smaller->height + 1;
    // Technically, this entire if-else can be reduced to a single call of rebalance_tree_up_ancestor_chain(ancestor_tracep)
    // But we will be more explicit.
    if (current->height == smaller->height)
    {
        // Case 2.1
        if (parent->height == current->height + 2)
            ;
        // Case 2.2
        else
            rebalance_tree_up_ancestor_chain(ancestor_tracep);
    }
    // Case 2.3
    else
        rebalance_tree_up_ancestor_chain(ancestor_tracep);
    return larger;
}

case_3:
{
    Stack ancestor_trace, *ancestor_tracep = &ancestor_trace;
    Stack_init(ancestor_tracep);
    TreeNode *current = smaller;
    while (current->height > larger->height)
    {
        Stack_push(ancestor_tracep, current);
        current = current->RIGHT;
    }

    TreeNode *parent = Stack_peek(ancestor_tracep);
    TreeNode *new_subroot = NewTreeNode(key);
    new_subroot->LEFT = current;
    new_subroot->RIGHT = larger;
    parent->RIGHT = new_subroot;
    new_subroot->height = larger->height + 1;

    if (current->height == larger->height)
    {
        if (parent->height == current->height + 2)
            ;
        else
            rebalance_tree_up_ancestor_chain(ancestor_tracep);
    }
    else
        rebalance_tree_up_ancestor_chain(ancestor_tracep);
    return smaller;
}
}

#define STACK_TYPE Key
#define STACK_MAX MAX_TREE_DEPTH
#define STACK_TYPE_NAMESPACE Key
#include "bounded_stack.h"
#undef STACK_TYPE_NAMESPACE
#undef STACK_MAX
#undef STACK_TYPE

// Splits the tree rooted at n to trees smaller and larger, where split_key is contained in the larger tree.
// In other words, (-inf, split_key) goes to smaller, [split_key, +inf) goes to larger.
void Tree_split(TreeNode *n, Key split_key, TreeNode **smaller, TreeNode **larger)
{
    Stack smaller_nlist, *smaller_nlistp = &smaller_nlist, larger_nlist, *larger_nlistp = &larger_nlist;
    Stack_Key smaller_klist, *smaller_klistp = &smaller_klist, larger_klist, *larger_klistp = &larger_klist;
    Stack_init(smaller_nlistp);
    Stack_init(larger_nlistp);
    Stack_init_Key(smaller_klistp);
    Stack_init_Key(larger_klistp);
    while (!TreeNode_IS_LEAF(n))
    {
        if (split_key < n->key)
        {
            Stack_push(larger_nlistp, n->RIGHT);
            Stack_push(larger_klistp, n->key);
            n = n->LEFT;
        }
        else
        {
            Stack_push(smaller_nlistp, n->LEFT);
            Stack_push(smaller_klistp, n->key);
            n = n->RIGHT;
        }
    }

    if (split_key < n->key)
        Stack_push(smaller_nlistp, n);
    else
        Stack_push(larger_nlistp, n);

    Key right_subtree_min_key;

    // make smaller tree
    TreeNode *smaller_right_subtree = Stack_pop(smaller_nlistp), *smaller_left_subtree;
    while (!Stack_empty(smaller_nlistp))
    {
        smaller_left_subtree = Stack_pop(smaller_nlistp);
        right_subtree_min_key = Stack_pop_Key(smaller_klistp);
        smaller_right_subtree = join_trees_with_known_key(smaller_left_subtree, smaller_right_subtree, right_subtree_min_key);
    }

    // make larger tree
    TreeNode *larger_left_subtree = Stack_pop(larger_nlistp), *larger_right_subtree;
    while (!Stack_empty(larger_nlistp))
    {
        larger_right_subtree = Stack_pop(larger_nlistp);
        right_subtree_min_key = Stack_pop_Key(larger_klistp);
        larger_left_subtree = join_trees_with_known_key(larger_left_subtree, larger_right_subtree, right_subtree_min_key);
    }

    *smaller = smaller_right_subtree;
    *larger = larger_left_subtree;
}

// Prints all leaf nodes.
void Tree_inorder_print(const TreeNode *node)
{
    if (TreeNode_IS_LEAF(node))
        printf("%d ", node->key);
    else
    {
        Tree_inorder_print(node->LEFT);
        Tree_inorder_print(node->RIGHT);
    }
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

#define NUM_TRIALS 10

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

    puts("Testing Tree_find, Tree_delete");
    for (int i = 0; i < NUM_TRIALS; i++)
    {
        TestObject *o = (TestObject *)Tree_find(tree, keys[i]);
        // printf("[%d]%d ", o->insertion_seq_num, o->key);
        assert(o->key == keys[i] && o->insertion_seq_num == i);
        o = Tree_delete(tree, keys[i]);
        assert(o->key == keys[i] && o->insertion_seq_num == i);
    }
    assert(Tree_IS_EMPTY(tree));
    puts("Test tree free");
    for (int i = 0; i < NUM_TRIALS; i++)
    {
        do
        {
            r = random() % KEY_MAX;
        } while (Tree_find(tree, r) != NULL_OBJECT);
        keys[i] = r;
        Tree_insert(tree, r, NewTestObject(r, i));
    }
    Tree_free(tree);
    tree = NULL;
#undef NUM_TRIALS
    puts("Tests passed");
    return 0;
}
