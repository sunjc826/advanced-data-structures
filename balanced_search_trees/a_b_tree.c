#include <stdio.h>
typedef void *key_t;
typedef void *object_t;
#define B 1000
typedef struct tr_n_t
{
    int degree; // a <= degree <= B
    int height; // for checking if node is leaf
    key_t key[B];
    struct tr_n_t *next[B];
} TreeNode;

object_t *find(TreeNode *tree, key_t query_key)
{
    TreeNode *current_node;
    object_t *object;
    current_node = tree;
    while (current_node->height >= 0)
    {
        int lower;
        int upper;
        lower = 0;
        upper = current_node->degree;
        // binary search
        while (upper > lower + 1)
        {
            if (query_key < current_node->key[(upper + lower) / 2])
            {
                upper = (upper + lower) / 2;
            }
            else
            {
                lower = (upper + lower) / 2;
            }
        }
        if (current_node->height > 0)
        {
            current_node = current_node->next[lower];
        }
        else
        {
            if (current_node->key[lower] == query_key)
            {
                object = (object_t *)current_node->next[lower];
            }
            else
            {
                object = NULL;
            }
            return object;
        }
    }
}
