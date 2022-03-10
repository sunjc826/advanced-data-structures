#include <stdio.h>

typedef void *item_t;
typedef struct
{
    item_t *base;
    int size;
    int max_size;
    item_t *copy;
    int copy_size;
} Stack;

Stack *create_stack(int size)
{
    Stack *stack;
    stack = (Stack *)malloc(sizeof(Stack));
    stack->base = (item_t *)malloc(size * sizeof(item_t));
    stack->max_size = size;
    stack->size = 0;
    stack->copy = NULL;
    stack->copy_size = 0;
    return stack;
}

int stack_empty(Stack *stack)
{
    return (stack->size == 0);
}

void push(Stack *stack, item_t item)
{
    *(stack->base + stack->size) = item;
    stack->size += 1;
    if (stack->copy != NULL || stack->size >= 0.75 * stack->max_size)
    {
        int additional_copies = 4;
        if (stack->copy == NULL)
        {
            stack->copy = (item_t *)malloc(2 * stack->max_size * sizeof(item_t));
        }
        // at most 4 copies per push operation
        // note that the copying only starts when stack size >= 0.75 of capacity
        // we can prove mathematically that when the stack becomes full, the copying will be complete
        while (additional_copies > 0 && stack->copy_size < stack->size)
        {
            *(stack->copy + stack->copy_size) = *(stack->base + stack->copy_size);
            stack->copy_size += 1;
            additional_copies -= 1;
        }
        if (stack->copy_size == stack->size)
        {
            free(stack->base);
            stack->base = stack->copy;
            stack->max_size *= 2;
            stack->copy = NULL;
            stack->copy_size = 0;
        }
    }
}

item_t pop(Stack *stack)
{
    item_t temp_item;
    stack->size -= 1;
    temp_item = *(stack->base + stack->size);
    if (stack->copy_size == stack->size)
    {
        free(stack->base);
        stack->base = stack->copy;
        // Notice that we do not delete nodes from the shadow copy
        // since the max_size handles the capacity.
        stack->max_size *= 2;
        stack->copy = NULL;
        stack->copy_size = 0;
    }
    return temp_item;
}

item_t top_element(Stack *stack)
{
    return (*(stack->base + stack->size - 1));
}

void remove_stack(Stack *stack)
{
    free(stack->base);
    if (stack->copy != NULL)
    {
        free(stack->copy);
    }
    free(stack);
}
