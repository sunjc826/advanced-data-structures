typedef void *key_t;
typedef struct tr_n_t
{
    key_t key;
    struct tr_n_t *left;
    struct tr_n_t *right;
    /* possibly additional info */
} TreeNode;
