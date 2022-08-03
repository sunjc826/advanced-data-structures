#ifndef ads_memory_allocator_h
#define ads_memory_allocator_h
typedef int Item;
typedef struct QueueNode_st
{
    Item item;
    struct QueueNode_st *next;
} QueueNode; // This can be any kind of node like structure, StackNode, etc.
QueueNode *QueueNode_get(void);
void QueueNode_return(QueueNode *);
#endif
