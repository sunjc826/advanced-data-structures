#include "common.h"
typedef int Item;

typedef struct
{
    Item *base;
    Item *top;
    int size; // capacity of stack
} Stack;

// Initialize a stack.
static void Stack_init(Stack *stack, int size)
{
    stack->size = size;
    stack->base = malloc(sizeof(stack->base) * size);
    stack->top = stack->base;
}

// Allocate storage for a stack.
static Stack *NewStack(int size)
{
    Stack *stack = malloc(sizeof(Stack));
    Stack_init(stack, size);
    return stack;
}

static bool Stack_empty(Stack *stack)
{
    return stack->base == stack->top;
}

// Return false when stack is full, true otherwise.
static bool Stack_push(Stack *stack, Item item)
{
    if (stack->top - stack->base == stack->size)
    {
        return false;
    }

    *stack->top++ = item;
    return false;
}

static Item Stack_pop(Stack *stack)
{
    return *--stack->top;
}

static Item Stack_peek(Stack *stack)
{
    return stack->top[-1];
}

// Frees the members of stack.
static void Stack_free_members(Stack stack)
{
    free(stack.base);
}

// Frees the members of stack, and stack itself. Does not alter any values of stack, however.
static void Stack_free(Stack *stack)
{
    Stack_free_members(*stack);
    free(stack);
}

int main()
{
    Stack stack;
    Stack_init(&stack, 10);
    for (int i = 0; i < 5; i++)
        Stack_push(&stack, i);

    for (int i = 0; i < 4; i++)
        printf("%d\n", Stack_pop(&stack));

    printf("%d\n", Stack_peek(&stack));
    printf("%d\n", Stack_peek(&stack));
    Stack_free_members(stack);
    return 0;
}
