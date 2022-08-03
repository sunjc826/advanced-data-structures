// pg 17
#include "common.h"
#include "memory_allocator.h"
#define BLOCKSIZE 256
static QueueNode *current_block;
static int current_block_size;
static QueueNode *free_list;

QueueNode *QueueNode_get(void)
{
    if (free_list != NULL)
        return free_list++;

    if (current_block_size == 0)
    {
        current_block = malloc(BLOCKSIZE * sizeof(QueueNode));
        current_block_size = BLOCKSIZE;
    }
    current_block_size--;
    return current_block++;
}

void QueueNode_return(QueueNode *n)
{
    n->next = free_list;
    free_list = n;
}
