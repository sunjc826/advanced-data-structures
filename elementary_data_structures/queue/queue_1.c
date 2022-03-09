#include <stdio.h>

typedef void *item_t;
typedef struct
{
    item_t *base;
    int front;
    int rear;
    int capacity;
} ArrayQueue;

ArrayQueue *create_queue(int capacity)
{
    ArrayQueue *queue;
    queue = (ArrayQueue *)malloc(sizeof(ArrayQueue));
    queue->base = (item_t *)malloc(capacity * sizeof(item_t));
    queue->capacity = capacity;
    queue->front = 0;
    queue->rear = 0;
    return queue;
}

int queue_empty(ArrayQueue *queue)
{
    return (queue->front == queue->rear);
}

int enqueue(ArrayQueue *queue, item_t item)
{
    // Not sure why the book uses `queue->rear + 2`
    if (queue->front == (queue->rear + 1) % queue->capacity)
    {
        return -1; // queue full
    }
    queue->base[queue->rear] = item;
    queue->rear = (queue->rear + 1) % queue->capacity;
    return 0;
}

item_t dequeue(ArrayQueue *queue)
{
    int temp;
    temp = queue->front;
    queue->front = (queue->front + 1) % queue->capacity;
    return (queue->base[temp]);
}

item_t front_element(ArrayQueue *queue)
{
    return (queue->base[queue->front]);
}

void remove_queue(ArrayQueue *queue)
{
    free(queue->base);
    free(queue);
}
