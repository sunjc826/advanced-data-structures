#include <stdio.h>

typedef void *item_t;
typedef struct qu_t
{
    item_t item;
    struct qu_t *next;
    struct qu_t *previous;
} CyclicDLLQueue; // doubly linked list
