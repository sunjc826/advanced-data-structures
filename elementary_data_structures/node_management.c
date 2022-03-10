#include <stdio.h>
#define BLOCKSIZE 256

typedef struct nd_t
{
    struct nd_t *next;
} Node;

Node *currentblock = NULL; // Contiguous block
int size_left;
Node *free_list = NULL; // Linked list

Node *get_node()
{
    Node *temp;
    if (free_list != NULL)
    {
        temp = free_list;
        free_list = free_list->next;
    }
    else
    {
        if (currentblock == NULL || size_left == 0)
        {
            currentblock = (Node *)malloc(BLOCKSIZE * sizeof(Node));
            size_left = BLOCKSIZE;
        }
        temp = currentblock;
        currentblock++;
        size_left -= 1;
    }
    return temp;
}

void return_node(Node *node)
{
    node->next = free_list;
    free_list = node;
}
