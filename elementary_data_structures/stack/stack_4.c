#include <stdio.h>

typedef void *item_t;
typedef struct stack
{
    item_t *base;
    item_t *top;
    int capacity;
    struct stack *previous;
} ListArrayStack; // i.e., a linked list of arrays

ListArrayStack *create_stack(int capacity)
{
    ListArrayStack *stack;
    stack = (ListArrayStack *)malloc(sizeof(ListArrayStack));
    stack->base = (item_t *)malloc(capacity * sizeof(item_t));
    stack->capacity = capacity;
    stack->top = stack->base;
    stack->previous = NULL;
    return stack;
}

void push(ListArrayStack *stack, item_t item)
{
    if (stack->top >= stack->base + stack->capacity) // the current array is full
    {
        ListArrayStack *new;
        new = (ListArrayStack *)malloc(sizeof(ListArrayStack));
        // Map the current array to new
        new->base = stack->base;
        new->top = stack->top;
        new->capacity = stack->capacity;
        new->previous = stack->previous;
        stack->previous = new;
        stack->base = (item_t *)malloc(stack->capacity * sizeof(item_t)); // the top array is now empty
        stack->top = stack->base;
    }
    // regardless of whether the if statement evaluates to true, we now have
    // an array at the top with free space
    *(stack->top) = item;
    stack->top += 1;
}

item_t pop(ListArrayStack *stack)
{
    if (stack->top == stack->base) // the top array is empty
    {
        ListArrayStack *old;
        old = stack->previous;
        stack->previous = old->previous;
        free(stack->base);
        stack->base = old->base;
        stack->top = old->top;
        stack->capacity = old->capacity;
        free(old); // This is tricky... Since we have copied over all fields
        // of `old` to `stack`, `old` can now be freed.
    }
    stack->top -= 1;
    return *(stack->top);
}

item_t top_element(ListArrayStack *stack)
{
    if (stack->top == stack->base)
    {
        return *(stack->previous->top - 1);
    }
    else
    {
        return *(stack->top - 1);
    }
}

void remove_stack(ListArrayStack *stack)
{
    ListArrayStack *temp;
    do
    {
        temp = stack->previous;
        free(stack->base);
        free(stack);
        stack = temp;
    } while (stack != NULL);
}
