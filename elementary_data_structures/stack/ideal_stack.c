#define INFINITY 1000000
typedef void *item_t;
int i = 0;
item_t stack[INFINITY];

int stack_empty()
{
    return i == 0;
}

void push(item_t item)
{
    stack[i] = item;
    i++;
}

item_t pop()
{
    i--;
    return stack[i];
}