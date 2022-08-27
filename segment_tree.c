#include "common.h"
#include "element_types.h"

// represents a half open interval [a, b)
typedef struct
{
    Endpoint a;
    Endpoint b;
} Interval;

typedef struct ListNode_st
{
    Interval *interval;
    struct ListNode_st *next;
} ListNode;

ListNode *NewListNode(Interval *interval)
{
    ListNode *n = malloc(sizeof(ListNode));
    n->interval = interval;
    n->next = NULL;
    return n;
}

typedef struct TreeNode_st
{
    Key key;
    struct TreeNode_st *left;
    struct TreeNode_st *right;
    ListNode *interval_list;
} TreeNode;

#define TreeNode_IS_LEAF(n) (n->right == NULL)

// Like the interval tree, there are 2 stages to building a segment tree.
// The first is to insert all endpoints. (and -inf) This requires implementing a BBST.
// The second is to insert the intervals.
// TODO: Do the first part next time.

void TreeNode_attach_interval(TreeNode *n, Interval *interval)
{
    ListNode *new_node = NewListNode(interval);
    new_node->next = n->interval_list;
    n->interval_list = new_node;
}

void Tree_insert_interval(TreeNode *root, Interval *interval)
{
    TreeNode *current = root, *left_path = NULL, *right_path = NULL;

    // invariant of the first loop
    // 1. interval is a strict subset of the interval associated with current
    // 2. Neither of interval's endpoints equal to any of current's interval's endpoints
    // Initially, this is trivially true since root's interval is (-inf, +inf)

    while (!TreeNode_IS_LEAF(current))
    {
        if (interval->b < current->key) // the loop invariant continues to hold, since b \neq key
            current = current->left;
        else if (current->key < interval->a) // the loop invariant continues to hold, since a \neq key
            current = current->right;
        else if (interval->a < current->key && current->key < interval->b) // the case where a split happens
        {
            left_path = current->left;
            right_path = current->right;
            break;
        }
        else if (current->key == interval->a)
        {
            right_path = current->right;
            break;
        }
        else // current->key == interval->b
        {
            left_path = current->left;
            break;
        }
    }

    if (left_path != NULL)
    {
        // invariant
        // left_path's interval's right endpoint is an element of [a, b], which is the topological closure of [a, b)

        while (!TreeNode_IS_LEAF(left_path))
        {
            if (left_path->key < interval->a)
                left_path = left_path->right;
            else
            {
                TreeNode_attach_interval(left_path->right, interval);
                if (left_path->key == interval->a)
                    break;
                else
                    left_path = left_path->left;
            }
        }

        // assuming that all intervals have endpoints as leaves of the tree,
        // the check left_path->key == interval->a is unnecessary
        if (TreeNode_IS_LEAF(left_path) && left_path->key == interval->a)
            TreeNode_attach_interval(left_path, interval);
    }

    if (right_path != NULL)
    {
        // invariant
        // right_path's interval's left endpoint is an element of [a, b)

        while (!TreeNode_IS_LEAF(right_path))
        {
            if (interval->b < right_path->key)
                right_path = right_path->left;
            else
            {
                TreeNode_attach_interval(right_path->left, interval);
                if (interval->b == right_path->key)
                    break;
                else
                    right_path = right_path->right;
            }
        }
    }
}
