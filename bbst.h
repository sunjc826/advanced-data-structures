#ifndef ADS_BBST_H
#define ADS_BBST_H
#include "common.h"
typedef int Key;
typedef struct TreeNode_st TreeNode;
typedef void *Object;
TreeNode *NewTree(void);
void Tree_free(TreeNode *n);
Object Tree_find(TreeNode *n, Key key);
bool Tree_insert(TreeNode *n, Key key, Object object);
Object Tree_delete(TreeNode *n, Key key);
Key Tree_min(TreeNode *n);
TreeNode *join_trees(TreeNode *smaller, TreeNode *larger);
TreeNode *join_trees_with_known_key(TreeNode *smaller, TreeNode *larger, Key key);
void Tree_split(TreeNode *n, Key split_key, TreeNode **smaller, TreeNode **larger);
void Tree_inorder_print(const TreeNode *node);
#endif
