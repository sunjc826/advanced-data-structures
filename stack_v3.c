// pg 4
#include "common.h"

typedef int Item;
#define NULL_ITEM 0
typedef struct StackNode_st // the st suffix stands for struct
{
    Item item;
    struct StackNode_st *next;
} StackNode;

// Memory management ownership rules:
// All nodes in a stack are owned by the sentinel node.
// Hence, when freeing any other node, there is no need to free the next node.
// But when freeing the sentinel, all nodes in the stack are freed.

static void StackNode_init(StackNode *node, Item item)
{
    node->item = item;
    node->next = NULL;
}

static StackNode *NewStackNode(Item item)
{
    StackNode *n = malloc(sizeof(StackNode));
    StackNode_init(n, item);
    return n;
}

static void Stack_init(StackNode *stack)
{
    StackNode_init(stack, NULL_ITEM);
}

// Creates a new Stack, more accurately, the sentinel node of the stack.
static StackNode *NewStack(void)
{
    StackNode *stack = NewStackNode(NULL_ITEM);
    return stack;
}

// Does not free the linked next node.
static void StackNode_free(StackNode *n)
{
    free(n);
}

static void Stack_free(StackNode *stack)
{
    StackNode *cur_node, *next_node;
    for (cur_node = stack; cur_node != NULL; cur_node = next_node)
    {
        next_node = cur_node->next;
        StackNode_free(cur_node);
    }
}

static bool Stack_empty(StackNode *stack)
{
    return stack->next == NULL;
}

static Item Stack_peek(StackNode *stack)
{
    return stack->next->item;
}

static Item Stack_pop(StackNode *stack)
{
    StackNode *top = stack->next;
    Item item = top->item;
    stack->next = top->next;
    StackNode_free(top);
    return item;
}

static void Stack_push(StackNode *stack, Item item)
{
    StackNode *new_top = NewStackNode(item);
    new_top->next = stack->next;
    stack->next = new_top;
}

typedef StackNode Stack;

int main()
{
    Stack *stack = NewStack();
    for (int i = 0; i < 5; i++)
        Stack_push(stack, i);

    for (int i = 0; i < 4; i++)
        printf("%d\n", Stack_pop(stack));

    printf("%d\n", Stack_peek(stack));
    printf("%d\n", Stack_peek(stack));
    Stack_free(stack);
    stack = NULL;
    return 0;
}
