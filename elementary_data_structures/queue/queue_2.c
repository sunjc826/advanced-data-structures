#include <stdio.h>

typedef void *item_t;
typedef struct qu_n_t
{
    item_t item;
    struct qu_n_t *next;
} QueueNode;

// This differs from the stack_3.c implementation since this has
// two structs, one for the node, and one for the queue.
// Alternatively, a queue can also be implemented using 2 sentinels,
// one at the front and one at the back.
typedef struct
{
    QueueNode *remove; // head
    QueueNode *insert; // tail
} ListQueue;

// Unimplemented
QueueNode *get_node()
{
}

// Unimplemented
// Frees the given node in memory.
void return_node(QueueNode *node)
{
}

ListQueue *create_queue()
{
    ListQueue *queue;
    queue = (ListQueue *)malloc(sizeof(ListQueue));
    queue->remove = NULL;
    queue->insert = NULL;
}

int queue_empty(ListQueue *queue)
{
    return (queue->insert == NULL);
}

void enqueue(ListQueue *queue, item_t item)
{
    QueueNode *temp;
    temp = get_node();
    temp->item = item;
    temp->next = NULL;
    if (queue->insert = NULL) // empty queue
    {
        queue->remove = temp;
        queue->insert = temp;
    }
    else
    {
        queue->insert->next = temp;
        queue->insert = temp;
    }
}

// We assume that we are dequeueing from a non-empty queue.
item_t dequeue(ListQueue *queue)
{
    QueueNode *temp;
    item_t temp_item;
    temp = queue->remove;
    temp_item = temp->item;
    queue->remove = temp->next;
    if (queue->remove == NULL)
    {
        queue->insert = NULL;
    }
    return_node(temp);
    return temp_item;
}

// We assume that the queue is non-empty.
item_t front_element(ListQueue *queue)
{
    return (queue->remove->item);
}

void remove_queue(ListQueue *queue)
{
    QueueNode *temp;
    while (queue->remove != NULL)
    {
        temp = queue->remove;
        queue->remove = temp->next;
        return_node(temp);
    }
    free(queue);
}
