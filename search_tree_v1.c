// The following BST is a model 1 search tree supporting operations
// Find O(h)
// Insert O(h)
// Delete O(h)
// where h := height of tree
// All objects must have unique keys.
#include "common.h"

typedef int Key;
#define NULL_KEY 0
#define NULL_OBJECT NULL
#define KEY_MAX INT_MAX
typedef void *Object;
typedef struct TreeNode_st
{
    Key key;
    union
    {
        struct TreeNode_st *node;
        Object object;
    } left;
    struct TreeNode_st *right;
} TreeNode;

// Ownership rules:
// TreeNode does not own its object. Likewise, a tree does not own any of the objects.

#define LEFT left.node
#define OBJECT left.object
#define RIGHT right
#define Tree_IS_EMPTY(treenode) (((TreeNode *)treenode)->LEFT == NULL)
#define TreeNode_IS_LEAF(treenode) (((TreeNode *)treenode)->RIGHT == NULL)

#define SWAP(type, a, b) \
    do                   \
    {                    \
        type tmp = a;    \
        a = b;           \
        b = tmp;         \
    } while (false);

static void TreeNode_init(TreeNode *n, Key key)
{
    n->key = key;
    n->LEFT = NULL;
    n->RIGHT = NULL;
}

static TreeNode *NewTreeNode(Key key)
{
    TreeNode *n = malloc(sizeof(TreeNode));
    TreeNode_init(n, key);
    return n;
}

// Does not free the object.
static void TreeNode_free(TreeNode *n)
{
    free(n);
}

static TreeNode *NewTreeNodeLeaf(Key key, Object object)
{
    TreeNode *n = NewTreeNode(key);
    n->OBJECT = object;
    return n;
}

static TreeNode *NewTree(void)
{
    TreeNode *empty_root = NewTreeNode(NULL_KEY);
    return empty_root;
}

// Moves nodes on the right of n to the left and maintains a valid BST.
// n, n->right must be interior nodes.
static void Tree_left_rotate(TreeNode *n)
{
    SWAP(Key, n->key, n->RIGHT->key);
    TreeNode *right = n->RIGHT;
    n->RIGHT = right->RIGHT;
    right->RIGHT = right->LEFT;
    right->LEFT = n->LEFT;
    n->LEFT = right;
}

// Moves nodes on the left of n to the right and maintains a valid BST.
// n, n->left must be interior nodes.
static void Tree_right_rotate(TreeNode *n)
{
    SWAP(Key, n->key, n->LEFT->key);
    TreeNode *left = n->LEFT;
    n->LEFT = left->LEFT;
    left->LEFT = left->RIGHT;
    left->RIGHT = n->RIGHT;
    n->RIGHT = left;
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

    return (n->key == key) ? n->OBJECT : NULL_OBJECT;
}

// recursive version
static Object Tree_find_v2(TreeNode *n, Key key)
{
    if (Tree_IS_EMPTY(n))
        return NULL_OBJECT;
    if (TreeNode_IS_LEAF(n))
        return (n->key == key) ? n->OBJECT : NULL_OBJECT;

    TreeNode *child = (key < n->key) ? n->LEFT : n->RIGHT;
    return Tree_find_v2(child, key);
}

static void TreeNode_write_leaf(TreeNode *n, Key key, Object object)
{
    n->key = key;
    n->OBJECT = object;
    n->RIGHT = NULL;
}

// Returns true on success, false on failure.
static bool Tree_insert(TreeNode *n, Key key, Object object)
{
    if (Tree_IS_EMPTY(n))
    {
        TreeNode_write_leaf(n, key, object);
        return true;
    }

    while (!TreeNode_IS_LEAF(n))
    {
        if (key < n->key)
            n = n->LEFT;
        else
            n = n->RIGHT;
    }

    if (n->key == key)
    {
        fputs("Keys should be unique.", stderr);
        return false;
    }

    TreeNode *interior_node = n;
    TreeNode *old_leaf_node = NewTreeNodeLeaf(n->key, n->OBJECT);
    TreeNode *new_leaf_node = NewTreeNodeLeaf(key, object);
    if (key < n->key)
    {
        // interior_node->key == n->key already holds
        interior_node->LEFT = new_leaf_node;
        interior_node->RIGHT = old_leaf_node;
    }
    else // n->key < key
    {
        interior_node->key = key;
        interior_node->LEFT = old_leaf_node;
        interior_node->RIGHT = new_leaf_node;
    }
    return true;
}

// Returns the Object with the given key. Returns NULL_OBJECT if not found.
// Note: The Object is returned not just for convenience. Since TreeNode does not own the object,
// the object is returned to avoid a memory leak. The caller of this function may then free
// the object.
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

    TreeNode *parent; // a "back-pointer" as we traverse the tree
    while (!TreeNode_IS_LEAF(n))
    {
        parent = n;
        if (key < n->key)
            n = n->LEFT;
        else
            n = n->RIGHT;
    }

    if (n->key != key)
        return NULL_OBJECT;

    object = n->OBJECT;

    TreeNode *sibling = (n == parent->LEFT) ? parent->RIGHT : parent->LEFT;

    parent->key = sibling->key;
    parent->left = sibling->left; // Since we don't know if sibling is leaf or interior node.
    parent->right = sibling->right;

    TreeNode_free(n);
    TreeNode_free(sibling);

    return object;
}

#define STACK_TYPE TreeNode *
#define STACK_TYPE_NAMESPACE TreeNode
#define NULL_ITEM NULL
#include "stack.h"
#undef NULL_ITEM
#undef STACK_TYPE_NAMESPACE
#undef STACK_TYPE

// Returns all objects whose keys lie in the half open interval [low, high).
TreeNode *Tree_interval_find(TreeNode *n, Key low, Key high)
{
    TreeNode *linked_list = NULL;
    StackNode_TreeNode *stack = NewStack_TreeNode();
    Stack_push_TreeNode(stack, n);
    while (!Stack_empty_TreeNode(stack))
    {
        TreeNode *node = Stack_pop_TreeNode(stack); // We are repurposing the TreeNode datastructure as a linkedlist node
        if (TreeNode_IS_LEAF(node))
        {
            if (low <= node->key && node->key < high)
            {
                TreeNode *linked_list_node = NewTreeNodeLeaf(node->key, node->OBJECT);
                linked_list_node->RIGHT = linked_list;
                linked_list = linked_list_node;
            }
            continue;
        }

        if (high <= node->key) // [low, high) is disjoint from interval associated with node->RIGHT
            Stack_push_TreeNode(stack, node->LEFT);
        else if (low >= node->key) // [low, high) is disjoint from interval associated with node->LEFT
            Stack_push_TreeNode(stack, node->RIGHT);
        else // low < node->key <= high
        {
            Stack_push_TreeNode(stack, node->LEFT);
            Stack_push_TreeNode(stack, node->RIGHT);
        }
    }

    Stack_free_TreeNode(stack);
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
#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)
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

    puts("Testing Tree_interval_find");
    int a = 0;
    int b = NUM_TRIALS - 1;
    int low = MIN(keys[a], keys[b]);
    int high = MAX(keys[a], keys[b]);
    TreeNode *linked_list, *scanner, *scanner_lookahead;
    linked_list = Tree_interval_find(tree, low, high);
    printf("low: %d, high: %d\n", low, high);
    for (scanner = linked_list; scanner != NULL; scanner = scanner_lookahead)
    {
        printf("%d ", scanner->key);
        scanner_lookahead = scanner->RIGHT;

        if (scanner_lookahead != NULL) // i.e. the returned linked list is sorted in increasing key order
            assert(scanner->key < scanner_lookahead->key);
        assert(low <= scanner->key && scanner->key < high);
    }
    puts("");

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
#undef MAX
#undef MIN
#undef NUM_TRIALS

    puts("Tests passed");
    return 0;
}
