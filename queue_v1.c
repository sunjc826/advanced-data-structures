// pg 9
#include "common.h"
typedef int Item;
typedef struct
{
    Item *base;
    int front;
    int rear;
    int size;
} Queue; // circularly bounded queue

#define PREV(queue, index) (((int)(index)-1) % ((Queue)(queue)).size)
#define NEXT(queue, index) (((int)(index) + 1) % ((Queue)(queue)).size)

static void Queue_init(Queue *queue, int size)
{
    queue->base = malloc(sizeof(queue->base) * size);
    queue->front = 0;
    queue->rear = 0;
    queue->size = size;
}

static Queue *NewQueue(int size)
{
    Queue *queue = malloc(sizeof(Queue));
    Queue_init(queue, size);
    return queue;
}

static void Queue_free_members(Queue queue)
{
    free(queue.base);
}

static void Queue_free(Queue *queue)
{
    Queue_free_members(*queue);
    free(queue);
}

static bool Queue_empty(Queue *queue)
{
    return queue->rear == queue->front;
}

// Returns false when queue is full, true otherwise.
static bool Queue_enqueue(Queue *queue, Item item)
{
    if (NEXT(*queue, queue->rear) == queue->front)
        return false;

    queue->base[queue->rear] = item;
    queue->rear = NEXT(*queue, queue->rear);
    return true;
}

static Item Queue_dequeue(Queue *queue)
{
    Item front = queue->base[queue->front];
    queue->front = NEXT(*queue, queue->front);
    return front;
}

static Item Queue_peek_front(Queue *queue)
{
    return queue->base[queue->front];
}

int main()
{
    Queue *queue = NewQueue(10);
    for (int i = 0; i < 9; i++)
        assert(Queue_enqueue(queue, i));
    for (int i = 0; i < 9; i++)
        printf("%d\n", Queue_dequeue(queue));
    Queue_free(queue);
    return 0;
}
