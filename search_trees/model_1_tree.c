#include <stdio.h>

typedef void *key_t;
typedef void *object_t;
typedef struct tr_n_t
{
    key_t key;
    struct tr_n_t *left;
    struct tr_n_t *right;
    /* possibly additional info */
} TreeNode;

// Unimplemented
TreeNode *get_node()
{
    return NULL;
}

// Unimplemented
void return_node(TreeNode *node)
{
}

TreeNode *create_tree()
{
    TreeNode *temp_node;
    temp_node = get_node();
    temp_node->left = NULL;
    return temp_node;
}

void left_rotation(TreeNode *node)
{
    TreeNode *temp_node;
    key_t temp_key;
    // store a reference to the left node, we don't need to
    // touch this till the very end
    temp_node = node->left;
    temp_key = node->key;
    node->left = node->right;
    node->key = node->right->key;
    node->right = node->left->right;
    node->left->right = node->left->left;
    node->left->left = temp_node;
    node->left->key = temp_key;
}

void right_rotation(TreeNode *node)
{
    TreeNode *temp_node;
    key_t temp_key;
    temp_node = node->right;
    temp_key = node->key;
    node->right = node->left;
    node->key = node->left->key;
    node->left = node->right->left;
    node->right->left = node->right->right;
    node->right->right = temp_node;
    node->right->key = temp_key;
}

object_t *find(TreeNode *node, key_t query_key)
{
    TreeNode *temp_node;
    if (node->left == NULL) // empty tree
    {
        return NULL;
    }
    else
    {
        temp_node = node;
        while (temp_node->right != NULL)
        {
            if (query_key < temp_node->key)
            {
                temp_node = temp_node->left;
            }
            else
            {
                temp_node = temp_node->right;
            }
        }
        if (temp_node->key != query_key)
        {
            return NULL;
        }
        return ((object_t *)(temp_node->left));
    }
}

object_t *find_recursive(TreeNode *node, key_t query_key)
{
    if (node->left == NULL || (node->right == NULL && node->key != query_key))
    {
        return NULL;
    }
    else if (node->right == NULL && node->key == query_key)
    {
        return ((object_t *)(node->left));
    }
    else
    {
        if (query_key < node->key)
        {
            return find_recursive(node->left, query_key);
        }
        else
        {
            return find_recursive(node->right, query_key);
        }
    }
}

int insert(TreeNode *node, key_t new_key, object_t *new_object)
{
    TreeNode *temp_node;
    if (node->left == NULL)
    {
        node->left = (TreeNode *)new_object;
        node->key = new_key;
        node->right = NULL;
    }
    else
    {
        temp_node = node;
        while (temp_node->right != NULL)
        {
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
        new_leaf = get_node();
        new_leaf->left = (TreeNode *)new_object;
        new_leaf->key = new_key;
        new_leaf->right = NULL;
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
    }
    return 0;
}

object_t *delete (TreeNode *node, key_t delete_key)
{
    TreeNode *temp_node;
    TreeNode *upper_node;
    TreeNode *other_node;
    object_t *deleted_object;
    if (node->left == NULL)
    {
        return NULL;
    }
    else if (node->right == NULL)
    {
        if (node->key != delete_key)
        {
            return NULL;
        }
        deleted_object = (TreeNode *)node->left;
        node->left = NULL;
        return deleted_object;
    }
    else
    {
        temp_node = node;
        while (temp_node->right != NULL)
        {
            upper_node = temp_node;
            if (delete_key < temp_node->key)
            {
                temp_node = upper_node->left;
                other_node = upper_node->right;
            }
            else
            {
                temp_node = upper_node->right;
                other_node = upper_node->left;
            }
        }
        if (temp_node->key != delete_key)
        {
            return NULL;
        }
        // more copying is needed if TreeNode has other fields
        upper_node->key = other_node->key;
        upper_node->left = other_node->left;
        upper_node->right = other_node->right;
        deleted_object = (object_t *)temp_node->left;
        return_node(temp_node);
        return_node(other_node);
        return deleted_object;
    }
}
