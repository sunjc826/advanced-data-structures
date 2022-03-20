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

TreeNode *get_node();
void return_node(TreeNode *);

void create_stack();
int stack_empty();
int push(TreeNode *);
TreeNode *pop();
void remove_stack();

TreeNode *create_tree()
{
    TreeNode *temp;
    temp = get_node();
    temp->height = 0;
    temp->degree = 0;
    return temp;
}

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

int insert(TreeNode *tree, key_t new_key, object_t *new_object)
{
    TreeNode *current_node;
    TreeNode *insert_pt;
    key_t insert_key;
    int finished;
    current_node = tree;
    if (tree->height == 0 && tree->degree == 0)
    {
        tree->key[0] = new_key;
        tree->next[0] = (TreeNode *)new_object;
        tree->degree = 1;
        return 0;
    }
    create_stack();
    while (current_node->height > 0)
    {
        int lower;
        int upper;
        push(current_node);
        lower = 0;
        upper = current_node->degree;
        while (lower + 1 < upper)
        {
            if (new_key < current_node->key[(lower + upper) / 2])
            {
                upper = (lower + upper) / 2;
            }
            else
            {
                lower = (lower + upper) / 2;
            }
        }
        current_node = current_node->next[lower];
    }

    insert_pt = (TreeNode *)new_object;
    insert_key = new_key;
    finished = 0;
    while (!finished)
    {
        int i;
        int start; // index of lowest key
        if (current_node->height > 0)
        {
            start = 1; // non-leaf node keys lie in indices 1 to B-1
        }
        else
        {
            start = 0; // leaf keys lie in indices 0 to B-1
        }
        // case (a)
        if (current_node->degree < B)
        {
            i = current_node->degree;
            // this is similar to insertion sort's inner loop
            // notice that for non-leaf nodes, insertion at index 0 never occurs
            // to show that this is correct, we need to consider all cases where we would need to insert inner node, i.e. splitting, sharing, joining
            // for splitting, see the comment below: we always insert the upper half of a split node, so the upper half `->next` index >= 1
            while ((i > start) && (current_node->key[i - 1] > insert_key))
            {
                current_node->key[i] = current_node->key[i - 1];
                current_node->next[i] = current_node->next[i - 1];
                i -= 1;
            }
            current_node->key[i] = insert_key;
            current_node->next[i] = insert_pt;
            current_node->degree += 1;
            finished = 1;
        }
        // case (b): Overfull, hence need to conduct operation: split
        else
        {
            TreeNode *new_node;
            int j;
            int insert_done = 0;
            new_node = get_node();
            i = B - 1;
            j = (B - 1) / 2;
            while (j >= 0)
            {
                if (insert_done || insert_key < current_node->key[i])
                {
                    new_node->next[j] = current_node->next[i];
                    new_node->key[j] = current_node->key[i];
                    j--;
                    i--;
                }
                else
                {
                    new_node->next[j] = insert_pt;
                    new_node->key[j] = insert_key;
                    j--;
                    insert_done = 1;
                }
            }
            // upper half has been moved out, but no insertion was made
            // i.e. insertion needs to be done in lower half
            while (!insert_done)
            {
                if (insert_key < current_node->key[i] && i >= start)
                {
                    current_node->next[i + 1] = current_node->next[i];
                    current_node->key[i + 1] = current_node->key[i];
                    i--;
                }
                else
                {
                    current_node->next[i + 1] = insert_pt;
                    current_node->key[i + 1] = insert_key;
                    insert_done = 1;
                }
            }

            current_node->degree = B + 1 - ((B + 1) / 2); // ceiling
            new_node->degree = (B + 1) / 2;
            new_node->height = current_node->height;
            // we always try to insert the upper half, so that its associated next index in the parent node >= 1.
            insert_pt = new_node;
            insert_key = new_node->key[0];
            // not splitting root
            if (!stack_empty())
            {
                current_node = pop();
            }
            // splitting root
            else
            {
                new_node = get_node();
                for (i = 0; i < current_node->degree; i++)
                {
                    new_node->next[i] = current_node->next[i];
                    new_node->key[i] = current_node->key[i];
                }
                new_node->height = current_node->height;
                new_node->degree = current_node->degree;
                current_node->height += 1;
                current_node->degree = 2;
                current_node->next[0] = new_node;
                current_node->next[1] = insert_pt;
                current_node->key[1] = insert_key;
                finished = 1;
            }
        }
    }
    remove_stack();
    return 0;
}
