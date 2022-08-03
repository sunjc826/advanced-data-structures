// pg 11
#include "common.h"
typedef int Item;
typedef struct QueueNode_st
{
    Item item;
    struct QueueNode_st *next;
} QueueNode;
typedef struct
{
    QueueNode *front; // remove here
    QueueNode *rear;  // insert here
} Queue;              // unlike a Stack, we need to keep track of both the front and back of the queue.

static void QueueNode_init(QueueNode *n, Item item)
{
    n->item = item;
    n->next = NULL;
}

static QueueNode *NewQueueNode(Item item)
{
    QueueNode *n = malloc(sizeof(QueueNode));
    QueueNode_init(n, item);
    return n;
}

static void Queue_init(Queue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
}

static Queue *NewQueue(void)
{
    Queue *queue = malloc(sizeof(Queue));
    Queue_init(queue);
    return queue;
}

static void QueueNode_free(QueueNode *n)
{
    free(n);
}

static void Queue_free_members(Queue queue)
{
    QueueNode *cur_node, *next_node;
    for (cur_node = queue.front; cur_node != NULL; cur_node = next_node)
    {
        next_node = cur_node->next;
        QueueNode_free(cur_node);
    }
}

static void Queue_free(Queue *queue)
{
    Queue_free_members(*queue);
    free(queue);
}

static bool Queue_empty(Queue *queue)
{
    return queue->front == NULL;
}

static void Queue_enqueue(Queue *queue, Item item)
{
    QueueNode *item_node = NewQueueNode(item);
    if (queue->rear == NULL)
        queue->front = queue->rear = item_node;
    else
    {
        queue->rear->next = item_node;
        queue->rear = item_node;
    }
}

// Assumes that the user of the queue ADT does not peek at an empty queue.
static Item Queue_peek_front(Queue *queue)
{
    return queue->front->item;
}

// Assumes that the user of the queue ADT does not dequeue from an empty queue.
static Item Queue_dequeue(Queue *queue)
{
    QueueNode *front = queue->front;
    Item item = front->item;
    queue->front = front->next;
    QueueNode_free(front);
    if (queue->front == NULL)
        queue->rear = NULL;
    return item;
}

int main()
{
    Queue *queue = NewQueue();
    for (int i = 0; i < 100; i++)
        Queue_enqueue(queue, i);
    for (int i = 0; i < 100; i++)
        printf("%d\n", Queue_dequeue(queue));
    Queue_free(queue);
    return 0;
}
