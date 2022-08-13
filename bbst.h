#ifndef ADS_BBST_H
#define ADS_BBST_H
#include "common.h"
#include "element_types.h"
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

TreeNode *NewTree(void);
void Tree_free(TreeNode *n);
Object Tree_find(TreeNode *n, Key key);
bool Tree_insert(TreeNode *n, Key key, Object object);
Object Tree_delete(TreeNode *n, Key key);
Object Tree_find_min(TreeNode *n);
TreeNode *join_trees(TreeNode *smaller, TreeNode *larger);
TreeNode *join_trees_with_known_key(TreeNode *smaller, TreeNode *larger, Key key);
void Tree_split(TreeNode *n, Key split_key, TreeNode **smaller, TreeNode **larger);
void Tree_inorder_print(const TreeNode *node);
#endif
