typedef void *key_t;
typedef void *object_t;

/*
Notice there is no wrapper structure around a TreeNode for the root.
This has a conceptual benefit in that each subnode of a tree node
is truly a subtree in itself. A tree is now much more symmetrical
and fractal-like.
*/

typedef struct tr_n_t
{
    key_t key;
    struct tr_n_t *left;
    struct tr_n_t *right;
    /* possibly additional info */
} TreeNode;

// Unimplemented
TreeNode *get_node();
void return_node(TreeNode *node);
