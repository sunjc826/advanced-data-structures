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

#define STACK_TYPE TreeNode *
#define STACK_TYPE_NAMESPACE TreeNode
#define NULL_ITEM NULL
#include "stack.h"
#undef NULL_ITEM
#undef STACK_TYPE_NAMESPACE
#undef STACK_TYPE

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

// Does not free any objects under leaf nodes.
static void Tree_free_v1(TreeNode *n)
{
    if (Tree_IS_EMPTY(n))
    {
        TreeNode_free(n);
        return;
    }

    StackNode_TreeNode stack;
    Stack_init_TreeNode(&stack);

    while (!Stack_empty_TreeNode(&stack))
    {
        TreeNode *node = Stack_pop_TreeNode(&stack);
        if (!TreeNode_IS_LEAF(node))
        {
            Stack_push_TreeNode(&stack, node->LEFT);
            Stack_push_TreeNode(&stack, node->RIGHT);
        }
        TreeNode_free(node);
    }
}

// Precondition: n, n->left must be interior nodes.
// Notes: A subroutine of Tree_free_v2. Unlike Tree_right_rotate, this changes the root, hence
// the return value is the new root after rotation.
// Note that this differs from the style of the other algorithms,
// which do not modify the root and so do not require a return value.
static TreeNode *Tree_right_rotate_v2(TreeNode *n)
{
    TreeNode *left = n->LEFT;
    n->LEFT = left->RIGHT;
    left->RIGHT = n;
    return left;
}

// pg 49
// Notes: A more elegant tree freeing algorithm making use of rotations. Does not use an auxilliary stack.
static void Tree_free_v2(TreeNode *n)
{
    if (Tree_IS_EMPTY(n))
    {
        TreeNode_free(n);
        return;
    }

    while (!TreeNode_IS_LEAF(n))
    {
        while (!TreeNode_IS_LEAF(n->LEFT))
            n = Tree_right_rotate_v2(n);
        TreeNode *right = n->right;
        TreeNode_free(n->LEFT);
        TreeNode_free(n);
        n = right;
    }
    TreeNode_free(n);
}

// Description: Moves nodes on the right of n to the left and maintains a valid BST.
// Precondition: n, n->right must be interior nodes.
static void Tree_left_rotate(TreeNode *n)
{
    SWAP(Key, n->key, n->RIGHT->key);
    TreeNode *right = n->RIGHT;
    n->RIGHT = right->RIGHT;
    right->RIGHT = right->LEFT;
    right->LEFT = n->LEFT;
    n->LEFT = right;
}

// Description: Moves nodes on the left of n to the right and maintains a valid BST.
// Precondition: n, n->left must be interior nodes.
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

// pg 39
// Returns all objects whose keys lie in the half open interval [low, high).
static TreeNode *Tree_interval_find(TreeNode *n, Key low, Key high)
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

// pg 40
// Constructs a relatively balanced (logarithmic height) tree from a sorted list in a bottom up manner.
// Similar to Tree_interval_find, a TreeNode is repurposed as a linked list node.
static TreeNode *make_tree_v1(TreeNode *sorted_list)
{
    // TODO
    return NULL;
}

// Constructs a relatively balanced tree from a sorted list in a top down manner.
static TreeNode *make_tree_v2(TreeNode *sorted_list)
{
    // TODO
    return NULL;
}

typedef struct MakeTreeV3_st
{
    TreeNode *node;
    int subtree_size;
    int offset; // offset into sorted array
} MakeTreeV3;

static const MakeTreeV3 MakeTreeV3_initializer;

#define QUEUE_TYPE MakeTreeV3
#define QUEUE_TYPE_NAMESPACE MakeTreeV3
#define NULL_ITEM MakeTreeV3_initializer
#include "queue.h"
#undef NULL_ITEM
#undef QUEUE_TYPE_NAMESPACE
#undef QUEUE_TYPE

// This is not in the book.
// Constructs a relatively balanced tree from a sorted array in a top down manner.
// Having a sorted array should make things much simpler. In fact, while make_tree_v2 is extremely
// elegant, a much simpler way would be to reduce v2 to v3 by converting a linked list to an array.
static TreeNode *make_tree_v3(TreeNode *sorted_array, int length)
{
    QueueNode_MakeTreeV3 *queue = NewQueue_MakeTreeV3(); // A stack would also be fine, it doesn't really whether we use queue or stack here.
    TreeNode *root = NewTree();
    MakeTreeV3 initial = {
        .node = root,
        .offset = 0,
        .subtree_size = length,
    };

    Queue_enqueue_MakeTreeV3(queue, initial);
    while (!Queue_empty_MakeTreeV3(queue))
    {
        MakeTreeV3 queue_element = Queue_dequeue_MakeTreeV3(queue);
        TreeNode *node = queue_element.node;
        if (queue_element.subtree_size == 1) // leaf
            TreeNode_write_leaf(node, sorted_array[queue_element.offset].key, sorted_array[queue_element.offset].OBJECT);
        else // interior node
        {
            // We choose to let the right subtree be smaller than left when number of elements is odd
            int right_subtree_size = queue_element.subtree_size / 2;
            int left_subtree_size = queue_element.subtree_size - right_subtree_size;
            node->key = sorted_array[queue_element.offset + left_subtree_size].key;
            node->LEFT = NewTreeNode(NULL_KEY);
            node->RIGHT = NewTreeNode(NULL_KEY);
            MakeTreeV3 left_queue_element = {
                .node = node->LEFT,
                .offset = queue_element.offset,
                .subtree_size = left_subtree_size,
            };
            MakeTreeV3 right_queue_element = {
                .node = node->RIGHT,
                .offset = queue_element.offset + left_subtree_size,
                .subtree_size = right_subtree_size,
            };
            Queue_enqueue_MakeTreeV3(queue, left_queue_element);
            Queue_enqueue_MakeTreeV3(queue, right_queue_element);
        }
    }

    return root;
}

// pg 48
// Creates sorted list of the leaves. This is like an inverse operation of make_tree.
// Unlike the book, this does not free any nodes in the tree.
static TreeNode *Tree_make_list(TreeNode *n)
{
    if (Tree_IS_EMPTY(n))
        return NULL;

    StackNode_TreeNode *stack = NewStack_TreeNode();
    TreeNode *linked_list = NULL;
    Stack_push_TreeNode(stack, n);
    while (!Stack_empty_TreeNode(stack))
    {
        TreeNode *node = Stack_pop_TreeNode(stack);
        if (TreeNode_IS_LEAF(node))
        {
            TreeNode *linked_list_node = NewTreeNodeLeaf(node->key, node->OBJECT);
            linked_list_node->RIGHT = linked_list;
            linked_list = linked_list_node;
        }
        else
        {
            Stack_push_TreeNode(stack, node->LEFT);
            Stack_push_TreeNode(stack, node->RIGHT);
        }
    }
    Stack_free_TreeNode(stack);

    return linked_list;
}

// adapted from https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
// prefix_length does not include null terminator
// doesn't seem to work very well, unfortunately
static void Tree_print_helper(const char *prefix, int prefix_length, const TreeNode *node, bool isLeft)
{

    printf("%s", prefix);

    printf("%s", (isLeft ? "├──" : "└──"));

    // print the value of the node
    printf("%d\n", node->key);

    // enter the next tree level - left and right branch
    char *extension = (isLeft ? "│   " : "    ");
    int extension_length = strlen(extension);
    int extended_prefix_length = prefix_length + strlen(extension);
    char *left_prefix = malloc(extended_prefix_length + 1);
    memcpy(left_prefix, prefix, prefix_length);
    memcpy(left_prefix, extension, extension_length);
    left_prefix[extended_prefix_length] = '\0';
    char *right_prefix = malloc(extended_prefix_length + 1);
    memcpy(right_prefix, prefix, prefix_length);
    memcpy(right_prefix, extension, extension_length);
    right_prefix[extended_prefix_length] = '\0';
    if (!TreeNode_IS_LEAF(node))
    {
        Tree_print_helper(left_prefix, extended_prefix_length, node->LEFT, true);
        Tree_print_helper(right_prefix, extended_prefix_length, node->RIGHT, false);
    }
}

static void Tree_print(const TreeNode *node)
{
    assert(node != NULL);
    Tree_print_helper("", 0, node, false);
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

void test_make_tree(void)
{
    puts("Test make_tree");
#define NUM_NODES 100
    TreeNode sorted[NUM_NODES];
    for (int i = 0; i < NUM_NODES; i++)
        sorted[i].key = i;
    TreeNode *tree = make_tree_v3(sorted, NUM_NODES);
    Tree_inorder_print(tree);
#undef NUM_NODES
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
    Tree_free_v2(tree);
    tree = NULL;
#undef NUM_TRIALS

    test_make_tree();

    puts("Tests passed");
    return 0;
}
