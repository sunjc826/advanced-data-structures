#ifndef ads_stack_h
#define ads_stack_h

// based on stack_v3
#if defined STACK_TYPE && defined NULL_ITEM && defined STACK_TYPE_NAMESPACE
#define CONCAT2(a, b) a##b
#define CONCAT3_(a, b, c) a##b##c
#define CONCAT3(a, b, c) CONCAT3_(a, b, c)
#define WITH_TYPE(token) CONCAT3(token, _, STACK_TYPE_NAMESPACE)
#define WITH_TYPE_FUNC(token, func) CONCAT

typedef STACK_TYPE WITH_TYPE(Item);
typedef struct WITH_TYPE(StackNode)
{
    WITH_TYPE(Item)
    item;
    struct WITH_TYPE(StackNode) * next;
} WITH_TYPE(StackNode);

static void WITH_TYPE(StackNode_init)(WITH_TYPE(StackNode) * node, WITH_TYPE(Item) item)
{
    node->item = item;
    node->next = NULL;
}

static WITH_TYPE(StackNode) * WITH_TYPE(NewStackNode)(WITH_TYPE(Item) item)
{
    WITH_TYPE(StackNode) *n = malloc(sizeof(WITH_TYPE(StackNode)));
    WITH_TYPE(StackNode_init)
    (n, item);
    return n;
}

// Creates a new Stack, more accurately, the sentinel node of the stack.
static WITH_TYPE(StackNode) * WITH_TYPE(NewStack)(void)
{
    WITH_TYPE(StackNode) *stack = WITH_TYPE(NewStackNode)(NULL_ITEM);
    return stack;
}

// Does not free the linked next node.
// Also does not free the associated item.
static void WITH_TYPE(StackNode_free)(WITH_TYPE(StackNode) * n)
{
    free(n);
}

static void WITH_TYPE(Stack_free)(WITH_TYPE(StackNode) * stack)
{
    WITH_TYPE(StackNode) * cur_node, *next_node;
    for (cur_node = stack; cur_node != NULL; cur_node = next_node)
    {
        next_node = cur_node->next;
        WITH_TYPE(StackNode_free)
        (cur_node);
    }
}

static bool WITH_TYPE(Stack_empty)(WITH_TYPE(StackNode) * stack)
{
    return stack->next == NULL;
}

static WITH_TYPE(Item) WITH_TYPE(Stack_peek)(WITH_TYPE(StackNode) * stack)
{
    return stack->next->item;
}

static WITH_TYPE(Item) WITH_TYPE(Stack_pop)(WITH_TYPE(StackNode) * stack)
{
    WITH_TYPE(StackNode) *top = stack->next;
    WITH_TYPE(Item)
    item = top->item;
    stack->next = top->next;
    WITH_TYPE(StackNode_free)
    (top);
    return item;
}

static void WITH_TYPE(Stack_push)(WITH_TYPE(StackNode) * stack, WITH_TYPE(Item) item)
{
    WITH_TYPE(StackNode) *new_top = WITH_TYPE(NewStackNode)(item);
    new_top->next = stack->next;
    stack->next = new_top;
}

#undef WITH_TYPE
#endif

#endif
