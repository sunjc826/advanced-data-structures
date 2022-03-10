#include <stdio.h>

typedef void *item_t;
typedef struct qu_t
{
    item_t item;
    struct qu_t *next;
} CyclicListQueue;

CyclicListQueue *create_queue()
{
    CyclicListQueue *entrypoint;
    CyclicListQueue *placeholder;
    entrypoint = (CyclicListQueue *)malloc(sizeof(CyclicListQueue));
    placeholder = (CyclicListQueue *)malloc(sizeof(CyclicListQueue));
    entrypoint->next = placeholder;
    placeholder->next = placeholder;
    return entrypoint;
}

int queue_empty(CyclicListQueue *queue)
{
    return (queue->next == queue->next->next);
}

// Note that since this is a singly linked list, to support enqueue
// operations, queue->next points to the last node in the queue (in the non-empty case).
// If queue->next points to placeholder instead, we cannot enqueue.
void enqueue(CyclicListQueue *queue, item_t item)
{
    CyclicListQueue *temp;
    CyclicListQueue *new;
    new = get_node();
    new->item = item;
    temp = queue->next;
    // `new` is the new tail of the queue
    queue->next = new;
    // insert `new` between `temp` and `temp->next`
    new->next = temp->next;
    temp->next = new;
}

item_t dequeue(CyclicListQueue *queue)
{
    CyclicListQueue *temp;
    item_t temp_item;
    // queue->next is the last node of an non-empty queue
    // queue->next->next is the placeholder node
    // queue->next->next->next is the head of the queue.
    temp = queue->next->next->next;
    queue->next->next->next = temp->next;
    // Note that this implementation isn't completely free of edge cases.
    // For example, we have this.
    if (temp == queue->next) // if queue used to only have one element
    {
        queue->next = temp->next;
    }
    temp_item = temp->item;
    return_node(temp);
    return temp_item;
}

item_t front_element(CyclicListQueue *queue)
{
    return (queue->next->next->next->item);
}

void remove_queue(CyclicListQueue *queue)
{
    CyclicListQueue *temp;
    temp = queue->next->next;
    while (temp != queue->next)
    {
        queue->next->next = temp->next;
        return_node(temp);
        temp = queue->next->next;
    }
    return_node(queue->next);
    return_node(queue);
}
