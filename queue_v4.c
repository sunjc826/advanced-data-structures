// pg 13
#include "common.h"
typedef int Item;
#define NULL_ITEM 0
typedef struct QueueNode_st
{
    Item item;
    struct QueueNode_st *next;
} QueueNode;

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

static void Queue_init(QueueNode *queue)
{
    QueueNode *placeholder = NewQueueNode(NULL_ITEM);
    placeholder->next = placeholder; // circularly linked list
    queue->next = placeholder;
}

static QueueNode *NewQueue(void)
{
    QueueNode *queue = NewQueueNode(NULL_ITEM); // this node is the so-called "entrypoint" by the book
    Queue_init(queue);
    return queue;
}

static void QueueNode_free(QueueNode *n)
{
    free(n);
}

static void Queue_free_members(QueueNode queue)
{
    QueueNode *node_before_placeholder = queue.next; // note that in an empty queue, node_before_placeholder is the same as placeholder
    QueueNode *cur_node, *next_node;
    for (cur_node = node_before_placeholder; cur_node != node_before_placeholder; cur_node = next_node)
    {
        next_node = cur_node->next;
        QueueNode_free(cur_node);
    }
}

static void Queue_free(QueueNode *queue)
{
    Queue_free_members(*queue);
    free(queue);
}

static bool Queue_empty(QueueNode *queue)
{
    return queue->next == queue->next->next;
}

static void Queue_enqueue(QueueNode *queue, Item item)
{
    QueueNode *new_node = NewQueueNode(item);
    QueueNode *node_before_placeholder = queue->next;
    QueueNode *placeholder = node_before_placeholder->next;
    node_before_placeholder->next = new_node;
    new_node->next = placeholder;
    queue->next = new_node;
}

static Item Queue_peek_front(QueueNode *queue)
{
    // queue->next is before placeholder, and the rear of the queue
    // queue->next->next is placeholder
    // queue->next->next->next is the head of the queue
    return queue->next->next->next->item;
}

static Item Queue_dequeue(QueueNode *queue)
{
    QueueNode *tail = queue->next;
    QueueNode *placeholder = tail->next;
    QueueNode *head = placeholder->next;
    Item item = head->item;
    placeholder->next = head->next;
    // Special case: The head of the queue is the same as the tail
    if (tail == head)
        queue->next = placeholder;
    QueueNode_free(head);
    return item;
}

static void Queue_print(QueueNode *queue)
{
    QueueNode *node_before_placeholder = queue->next;
    QueueNode *placeholder = node_before_placeholder->next;
    QueueNode *cur_node, *next_node;
    putchar('[');
    for (cur_node = placeholder->next; cur_node != placeholder; cur_node = next_node)
    {
        next_node = cur_node->next;
        printf("%d ", cur_node->item);
    }
    puts("]");
}

typedef QueueNode Queue;

int main()
{
    Queue *queue = NewQueue();
    assert(Queue_empty(queue));
    for (int i = 0; i < 50; i++)
        Queue_enqueue(queue, i);
    Queue_print(queue);
    for (int i = 0; i < 50; i++)
        printf("%d\n", Queue_dequeue(queue));
    Queue_print(queue);
    assert(Queue_empty(queue));
    Queue_enqueue(queue, 999);
    printf("%d\n", Queue_peek_front(queue));
    Queue_print(queue);
    assert(!Queue_empty(queue));
    Queue_free(queue);
    return 0;
}
