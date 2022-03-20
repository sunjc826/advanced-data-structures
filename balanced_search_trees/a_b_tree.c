#include <stdio.h>
typedef void *key_t;
typedef void *object_t;
#define A 4
#define B 8
// these numbers are arbitrarily chosen, in this case we have a (4, 8)-tree

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
        // Not overfull
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
        // Overfull, hence need to conduct operation: split
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

void create_node_stack();
void create_index_stack();
int push_node_stack(TreeNode *);
int push_index_stack(int);
TreeNode *pop_node_stack();
int pop_index_stack();

object_t *delete (TreeNode *tree, key_t delete_key)
{
    TreeNode *current;
    TreeNode *temp_node;
    int finished;
    int i;
    int j;
    current = tree;
    create_node_stack();
    create_index_stack();
    while (current->height > 0)
    {
        int lower;
        int upper;
        lower = 0;
        while (lower + 1 < upper)
        {
            if (delete_key < current->key[(lower + upper) / 2])
            {
                upper = (lower + upper) / 2;
            }
            else
            {
                lower = (lower + upper) / 2;
            }
        }
        // as we traverse down the tree, we store both
        // the node and the index of its child to which we move
        push_index_stack(lower);
        push_node_stack(current);
        current = current->next[lower];
    }
    for (i = 0; i < current->degree; i++)
    {
        if (current->key[i] == delete_key)
        {
            break;
        }
    }
    // key does not exist
    if (i == current->degree)
    {
        return NULL;
    }
    else
    {
        object_t *del_object;
        del_object = (object_t *)current->next[i];
        current->degree -= 1;
        while (i < current->degree)
        {
            current->next[i] = current->next[i + 1];
            current->key[i] = current->key[i + 1];
            i++;
        }
        finished = 0;
        while (!finished)
        {
            // Not underfull
            if (current->degree >= A)
            {
                finished = 1;
            }
            // Underfull
            else
            {
                // current is root
                if (stack_empty())
                {
                    if (current->degree >= 2)
                    {
                        finished = 1;
                    }
                    else if (current->height == 0)
                    {
                        finished = 1;
                    }
                    else
                    {
                        // move the node at height 1 to root
                        // i.e. we are effectively collapsing the root layer
                        temp_node = current->next[0];
                        for (i = 0; i < temp_node->degree; i++)
                        {
                            current->next[i] = temp_node->next[i];
                            current->key[i] = temp_node->key[i];
                        }
                        current->degree = temp_node->degree;
                        current->height = temp_node->height;
                        return_node(temp_node);
                        finished = 1;
                    }
                }
                // non-root node
                else
                {
                    TreeNode *upper;
                    TreeNode *neighbor;
                    int curr;
                    upper = pop_node_stack();
                    curr = pop_index_stack();
                    // child node `current` associated with `curr` is not the last child of `upper`
                    if (curr < upper->degree - 1)
                    {
                        // since `current` is not last child, it has a right neighbor
                        neighbor = upper->next[curr + 1];
                        // sharing with right neighbor
                        if (neighbor->degree > A)
                        {
                            i = current->degree;
                            if (current->height > 0)
                            {
                                current->key[i] = upper->key[curr + 1]; // start of search interval
                            }
                            else
                            {
                                current->key[i] = neighbor->key[0]; // leaf key
                                neighbor->key[0] = neighbor->key[1];
                            }
                            current->next[i] = neighbor->next[0];
                            upper->key[curr + 1] = neighbor->key[1];
                            neighbor->next[0] = neighbor->next[1];
                            for (j = 2; j < neighbor->degree; j++)
                            {
                                neighbor->next[j - 1] = neighbor->next[j];
                                neighbor->key[j - 1] = neighbor->key[j];
                            }
                            neighbor->degree -= 1;
                            current->degree += 1;
                            // since sharing does not change number of nodes, we are done
                            // no more rebalancing required
                            finished = 1;
                        }
                        // join with right neighbor
                        else
                        {
                            i = current->degree;
                            if (current->height > 0)
                            {
                                current->key[i] = upper->key[curr + 1];
                            }
                            else
                            {
                                current->key[i] = neighbor->key[0];
                            }
                            current->next[i] = neighbor->next[0];
                            for (j = 1; j < neighbor->degree; j++)
                            {
                                i++;
                                current->next[i] = neighbor->next[j];
                                current->key[i] = neighbor->key[j];
                            }
                            current->degree = i + 1;
                            return_node(neighbor);
                            upper->degree -= 1;
                            i = curr + 1;
                            while (i < upper->degree)
                            {
                                upper->next[i] = upper->next[i + 1];
                                upper->key[i] = upper->key[i + 1];
                                i++;
                            }
                            current = upper; // propagate deletion upwards
                        }
                    }
                    // `current` is last child node of `upper`, hence no right neighbor
                    else
                    {
                        // a left neighbor must exist since A > 1
                        neighbor = upper->next[curr - 1];
                        // sharing with left neighbor
                        if (neighbor->degree > A)
                        {
                            for (j = current->degree; j > 1; j--)
                            {
                                current->next[j] = current->next[j - 1];
                                current->key[j] = current->key[j - 1];
                            }
                            current->next[1] = current->next[0];
                            i = neighbor->degree;
                            current->next[0] = neighbor->next[i - 1];
                            if (current->height > 0)
                            {
                                current->key[1] = upper->key[curr];
                            }
                            else
                            {
                                current->key[1] = current->key[0];
                                current->key[0] = neighbor->key[i - 1];
                            }
                            upper->key[curr] = neighbor->key[i - 1];
                            neighbor->degree -= 1;
                            current->degree += 1;
                            finished = 1;
                        }
                        // joining with left neighbor
                        else
                        {
                            i = neighbor->degree;
                            if (current->height > 0)
                            {
                                neighbor->key[i] = upper->key[curr];
                            }
                            else
                            {
                                neighbor->key[i] = current->key[0];
                            }
                            neighbor->next[i] = current->next[0];
                            for (j = 1; j < current->degree; j++)
                            {
                                i++;
                                neighbor->next[i] = current->next[j];
                                neighbor->key[i] = current->key[j];
                            }
                            neighbor->degree = i + 1;
                            return_node(current);
                            upper->degree -= 1;
                            current = upper;
                        }
                    }
                }
            }
        }
        return del_object;
    }
}
