#include <stdio.h>

typedef void *item_t;
typedef struct stack_t
{
    item_t item;
    struct stack_t *next;
} ListStack;

// Unimplemented
ListStack *get_node()
{
}

// Unimplemented
// Frees the given node in memory.
void return_node(ListStack *node)
{
}

ListStack *create_stack()
{
    // sentinel node
    // as described in the book, apart from usually being useful,
    // sentinel nodes act as an identifier for a stack
    // and are unchanged as operations are conducted on the stack
    ListStack *stack;
    stack = get_node();
    stack->next = NULL;
    return stack;
}

void push(ListStack *stack, item_t item)
{
    ListStack *temp;
    temp = get_node();
    temp->item = item;
    temp->next = stack->next;
    stack->next = temp;
}

item_t pop(ListStack *stack)
{
    ListStack *temp;
    item_t temp_item;
    // As mentioned in the book, it is assumed that the stack is non-empty, as in an idealized stack,
    // where the correct usage involves popping only when the stack is non-empty.
    // Hence, nothing is done about possible stack underflows.
    temp = stack->next; // top of the stack
    temp_item = temp->item;
    stack->next = temp->next; // the node below the original top of stack
    return_node(temp);
    return temp_item;
}

item_t top_element(ListStack *stack)
{
    return stack->next->item;
}

void remove_stack(ListStack *stack)
{
    ListStack *temp;
    do
    {
        temp = stack->next;
        return_node(stack);
        stack = temp;
    } while (temp != NULL);
}
