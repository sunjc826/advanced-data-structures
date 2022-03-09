typedef void *item_t;
typedef struct
{
    item_t *base;
    // top is always 1 position above the top element
    // In particular, when the stack is empty, top == base
    item_t *top;
    int capacity;
} PointerStack;

PointerStack *create_stack(int capacity)
{
    // PointerStack is still a contiguous array.
    PointerStack *ps = (PointerStack *)malloc(sizeof(PointerStack));
    ps->base = (item_t *)malloc(capacity * sizeof(item_t));
    ps->capacity = capacity;
    ps->top = ps->base;
    return ps;
}

int stack_empty(PointerStack *ps)
{
    // Note: we are comparing item_t* with item_t*, so this is
    // an address comparison.
    return (ps->base == ps->top);
}

int push(PointerStack *ps, item_t item)
{
    if (ps->top >= ps->base + ps->capacity)
    {
        // stack is full
        return -1;
    }

    *(ps->top) = item;
    ps->top += 1;
    return 0;
}

item_t pop(PointerStack *ps)
{
    ps->top -= 1;
    return *(ps->top);
}

// peek
item_t top_element(PointerStack *ps)
{
    return *(ps->top - 1);
}

void remove_stack(PointerStack *ps)
{
    free(ps->base);
    free(ps);
}
