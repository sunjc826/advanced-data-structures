// pg 6
#include "common.h"
typedef int Item;
typedef struct StackChunk_st
{
    Item *base;
    Item *top;
    int size; // capacity of the chunk
    struct StackChunk_st *previous;
} StackChunk;

// Ownership rules:
// The topmost StackChunk is both a standlone chunk and represents the stack itself.
// When StackChunk_free is called on it, it is treated as a standalone chunk.
// When Stack_free is called on it, it is treated as a stack and hence owns all the other chunks in the chain.

static void StackChunk_init(StackChunk *chunk, int size)
{
    chunk->size = size;
    chunk->base = malloc(sizeof(chunk->base) * size);
    chunk->top = chunk->base;
    chunk->previous = NULL;
}

static StackChunk *NewStackChunk(int size)
{
    StackChunk *chunk = malloc(sizeof(StackChunk));
    StackChunk_init(chunk, size);
    return chunk;
}

static void StackChunk_free_members(StackChunk chunk)
{
    free(chunk.base);
}

static void StackChunk_free(StackChunk *chunk)
{
    StackChunk_free_members(*chunk);
    free(chunk);
}

static StackChunk *NewStack(int size)
{
    StackChunk *stack = NewStackChunk(size);
    return stack;
}

static void Stack_free(StackChunk *stack)
{
    StackChunk *cur_chunk, *next_chunk;
    for (cur_chunk = stack; cur_chunk != NULL; cur_chunk = next_chunk)
    {
        next_chunk = cur_chunk->previous;
        StackChunk_free(cur_chunk);
    }
}

static bool Stack_empty(StackChunk *stack)
{
    if (stack->previous != NULL)
        return false;
    return stack->base == stack->top;
}

static Item Stack_peek(StackChunk *stack)
{
    return (stack->top > stack->base) ? stack->top[-1] : stack->previous->top[-1];
}

static void Stack_push(StackChunk *stack, Item item)
{
    if (stack->top - stack->base == stack->size)
    {
        StackChunk *new_chunk = NewStackChunk(stack->size);
        // exchange operation
        Item *new_base = new_chunk->base;
        new_chunk->base = stack->base;
        new_chunk->top = stack->top;
        new_chunk->previous = stack->previous;
        stack->base = new_base;
        stack->top = new_base;
        stack->previous = new_chunk;
    }
    // At this point, the StackChunk pointed to by stack has free space
    *stack->top++ = item;
}

static Item Stack_pop(StackChunk *stack)
{
    if (stack->top == stack->base)
    {
        StackChunk *previous = stack->previous;
        // exchange operation, since we cannot change the pointer, StackChunk *stack, which is pass by value
        // a ** pointer would work, but it's not very user friendly
        Item *previous_base = previous->base;
        Item *previous_top = previous->top;
        StackChunk *previous_previous = previous->previous;

        previous->base = stack->base;
        previous->top = stack->top;
        previous->previous = NULL; // this is not necessary, but is probably good programming practice to avoid unforseen side effects
        StackChunk_free(previous);

        stack->base = previous_base;
        stack->top = previous_top;
        stack->previous = previous_previous;
    }
    // At this point, the StackChunk pointed to by stack has at least 1 item
    // (assuming the user of the Stack ADT does not try to pop an empty stack)
    return *--stack->top;
}

typedef StackChunk Stack;

int main()
{
    Stack *stack = NewStack(8);
    for (int i = 0; i < 50; i++)
        Stack_push(stack, i);

    for (int i = 0; i < 25; i++)
        printf("%d\n", Stack_pop(stack));

    printf("%d\n", Stack_peek(stack));
    printf("%d\n", Stack_peek(stack));
    for (int i = 100; i < 110; i++)
        Stack_push(stack, i);
    while (!Stack_empty(stack))
        printf("%d\n", Stack_pop(stack));
    Stack_free(stack);
    stack = NULL;
    return 0;
}
