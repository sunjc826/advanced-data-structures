// a canonical implementation on an idealized machine has the following properties
// assumptions:
// - no pop on empty stack, in other words stack underflow is an erroneous usage of the stack
// - no stack overflow, since an idealized machine has infinite memory

// the following implementation uses finite memory
#include <stdbool.h>
#define MAX_SIZE 1000

typedef int Item;
static int i = 0; // i can be interpreted as the next free index
static Item stack[MAX_SIZE];

static bool stack_empty()
{
    return i == 0;
}

// returns false when stack is full, true otherwise
static bool push(Item item)
{
    if (i == MAX_SIZE)
    {
        return false;
    }
    stack[i++] = item;
    return true;
}

static Item pop()
{
    return stack[--i];
}
