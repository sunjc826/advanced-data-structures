#ifndef ads_queue_h
#define ads_queue_h
#include "common.h"
// based on queue_v4

#if defined QUEUE_TYPE && defined QUEUE_TYPE_NAMESPACE && defined NULL_ITEM
#define WITH_TYPE(token) CONCAT3(token, _, QUEUE_TYPE_NAMESPACE)
#define QUEUENODE WITH_TYPE(QueueNode)
#define ITEM WITH_TYPE(QueueItem)

typedef QUEUE_TYPE ITEM;
typedef struct QUEUENODE
{
    ITEM item;
    struct QUEUENODE *next;
} QUEUENODE;

#define QUEUENODE_INIT WITH_TYPE(QueueNode_init)
static void QUEUENODE_INIT(QUEUENODE *n, ITEM item)
{
    n->item = item;
    n->next = NULL;
}

#define NEWQUEUENODE WITH_TYPE(NewQueueNode)
static QUEUENODE *NEWQUEUENODE(ITEM item)
{
    QUEUENODE *n = malloc(sizeof(QUEUENODE));
    QUEUENODE_INIT(n, item);
    return n;
}

#define QUEUE_INIT WITH_TYPE(Queue_init)
static void QUEUE_INIT(QUEUENODE *queue)
{
    QUEUENODE *placeholder = NEWQUEUENODE(NULL_ITEM);
    placeholder->next = placeholder; // circularly linked list
    queue->next = placeholder;
}

#define NEWQUEUE WITH_TYPE(NewQueue)
static QUEUENODE *NEWQUEUE(void)
{
    QUEUENODE *queue = NEWQUEUENODE(NULL_ITEM); // this node is the so-called "entrypoint" by the book
    QUEUE_INIT(queue);
    return queue;
}

#define QUEUENODE_FREE WITH_TYPE(QueueNode_free)
static void QUEUENODE_FREE(QUEUENODE *n)
{
    free(n);
}

#define QUEUE_FREE_MEMBERS WITH_TYPE(Queue_free_members)
static void QUEUE_FREE_MEMBERS(QUEUENODE queue)
{
    QUEUENODE *node_before_placeholder = queue.next; // note that in an empty queue, node_before_placeholder is the same as placeholder
    QUEUENODE *cur_node, *next_node;
    for (cur_node = node_before_placeholder; cur_node != node_before_placeholder; cur_node = next_node)
    {
        next_node = cur_node->next;
        QUEUENODE_FREE(cur_node);
    }
}

#define QUEUE_FREE WITH_TYPE(Queue_free)
static void QUEUE_FREE(QUEUENODE *queue)
{
    QUEUE_FREE_MEMBERS(*queue);
    free(queue);
}

#define QUEUE_EMPTY WITH_TYPE(Queue_empty)
static bool QUEUE_EMPTY(QUEUENODE *queue)
{
    return queue->next == queue->next->next;
}

#define QUEUE_ENQUEUE WITH_TYPE(Queue_enqueue)
static void QUEUE_ENQUEUE(QUEUENODE *queue, ITEM item)
{
    QUEUENODE *new_node = NEWQUEUENODE(item);
    QUEUENODE *node_before_placeholder = queue->next;
    QUEUENODE *placeholder = node_before_placeholder->next;
    node_before_placeholder->next = new_node;
    new_node->next = placeholder;
    queue->next = new_node;
}

#define QUEUE_PEEK_FRONT WITH_TYPE(Queue_peek_front)
static ITEM QUEUE_PEEK_FRONT(QUEUENODE *queue)
{
    // queue->next is before placeholder, and the rear of the queue
    // queue->next->next is placeholder
    // queue->next->next->next is the head of the queue
    return queue->next->next->next->item;
}

#define QUEUE_DEQUEUE WITH_TYPE(Queue_dequeue)
static ITEM QUEUE_DEQUEUE(QUEUENODE *queue)
{
    QUEUENODE *tail = queue->next;
    QUEUENODE *placeholder = tail->next;
    QUEUENODE *head = placeholder->next;
    ITEM item = head->item;
    placeholder->next = head->next;
    // Special case: The head of the queue is the same as the tail
    if (tail == head)
        queue->next = placeholder;
    QUEUENODE_FREE(head);
    return item;
}

#undef QUEUE_DEQUEUE
#undef QUEUE_PEEK_FRONT
#undef QUEUE_ENQUEUE
#undef QUEUE_EMPTY
#undef QUEUE_FREE
#undef QUEUE_FREE_MEMBERS
#undef NEWQUEUE
#undef QUEUE_INIT
#undef NEWQUEUENODE
#undef QUEUENODE_INIT
#undef QUEUENODE
#undef ITEM
#undef WITH_TYPE
#endif
#endif
