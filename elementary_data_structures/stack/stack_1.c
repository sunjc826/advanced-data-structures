#define MAXSIZE 100

typedef void *item_t;
typedef struct
{
    int size;
    item_t stack[MAXSIZE];
} ArrayStack;

int stack_empty(ArrayStack *arr_stack)
{
    return (arr_stack->size == 0);
}

int push(ArrayStack *arr_stack, item_t item)
{
    if (arr_stack >= MAXSIZE)
    {
        return -1;
    }
    arr_stack->stack[arr_stack->size] = item;
    arr_stack->size++;
    return -1;
}

item_t pop(ArrayStack *arr_stack)
{
    arr_stack->size--;
    return arr_stack->stack[arr_stack->size];
}
