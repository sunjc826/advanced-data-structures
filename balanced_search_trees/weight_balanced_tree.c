#include <stdio.h>
#define ALPHA 0.288
#define EPSILON 0.005

typedef void *key_t;
typedef void *object_t;

typedef struct tr_n_t
{
    key_t key;
    struct tr_n_t *left;
    struct tr_n_t *right;
    int weight;
} TreeNode;

// Unimplemented
TreeNode *get_node();
void return_node(TreeNode *);
void left_rotation(TreeNode *);
void right_rotation(TreeNode *);

void create_stack();
void remove_stack();
int stack_empty();
int push(TreeNode *node);
TreeNode *pop();

int insert(TreeNode *tree, key_t new_key, object_t *new_object)
{
    TreeNode *temp_node;
    if (tree->left == NULL)
    {
        tree->left = (TreeNode *)new_object;
        tree->key = new_key;
        tree->weight = 1;
        tree->right = NULL;
    }
    else
    {
        create_stack();
        temp_node = tree;
        while (temp_node->right != NULL)
        {
            push(temp_node);
            if (new_key < temp_node->key)
            {
                temp_node = temp_node->left;
            }
            else
            {
                temp_node = temp_node->right;
            }
        }
        if (temp_node->key == new_key)
        {
            return -1;
        }
        TreeNode *old_leaf;
        TreeNode *new_leaf;
        old_leaf = get_node();
        old_leaf->left = temp_node->left;
        old_leaf->key = temp_node->key;
        old_leaf->right = NULL;
        old_leaf->weight = 1;
        new_leaf = get_node();
        new_leaf->left = (TreeNode *)new_object;
        new_leaf->key = new_key;
        new_leaf->right = NULL;
        new_leaf->weight = 1;
        if (temp_node->key < new_key)
        {
            temp_node->left = old_leaf;
            temp_node->right = new_leaf;
            temp_node->key = new_key;
        }
        else
        {
            temp_node->left = new_leaf;
            temp_node->right = old_leaf;
        }
        temp_node->weight = 2;
    }

    while (!stack_empty())
    {
        temp_node = pop();
        // In a weight balanced tree, weight of each nodes in stack is always changed
        temp_node->weight = temp_node->left->weight + temp_node->right->weight;
        if (temp_node->right->weight < ALPHA * temp_node->weight) // case 2
        {
            if (temp_node->left->left->weight > (ALPHA + EPSILON) * temp_node->weight) // case 2.1
            {
                right_rotation(temp_node);
                temp_node->right->weight =
                    temp_node->right->left->weight +
                    temp_node->right->right->weight;
            }
            else // case 2.2
            {
                left_rotation(temp_node->left);
                right_rotation(temp_node);
                temp_node->left->weight =
                    temp_node->left->left->weight +
                    temp_node->left->right->weight;
                temp_node->right->weight =
                    temp_node->right->left->weight +
                    temp_node->right->right->weight;
            }
        }
        else if (temp_node->left->weight < ALPHA * temp_node->weight) // case 3
        {
            if (temp_node->right->right->weight > (ALPHA + EPSILON) * temp_node->weight)
            {
                left_rotation(temp_node);
                temp_node->left->weight =
                    temp_node->left->left->weight +
                    temp_node->left->right->weight;
            }
            else
            {
                right_rotation(temp_node->right);
                left_rotation(temp_node);
                temp_node->left->weight =
                    temp_node->left->left->weight +
                    temp_node->left->right->weight;
                temp_node->right->weight =
                    temp_node->right->left->weight +
                    temp_node->right->right->weight;
            }
        }
    }
    remove_stack();
    return 0;
}
