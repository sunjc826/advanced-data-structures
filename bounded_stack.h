#include "common.h"
#if defined STACK_TYPE && defined STACK_MAX
#ifdef STACK_TYPE_NAMESPACE
#define WITH_TYPE(token) CONCAT3(token, _, STACK_TYPE_NAMESPACE)
#else
#define WITH_TYPE(token) token
#endif
#define STACK WITH_TYPE(Stack)
typedef struct
{
    STACK_TYPE arr[STACK_MAX];
    STACK_TYPE *next;
} STACK;

#define STACK_INIT WITH_TYPE(Stack_init)
static void STACK_INIT(STACK *stack)
{
    stack->next = stack->arr;
}

#define STACK_EMPTY WITH_TYPE(Stack_empty)
static bool STACK_EMPTY(STACK *stack)
{
    return stack->next == stack->arr;
}

#define STACK_PEEK WITH_TYPE(Stack_peek)
static STACK_TYPE STACK_PEEK(STACK *stack)
{
    return stack->next[-1];
}

#define STACK_POP WITH_TYPE(Stack_pop)
static STACK_TYPE STACK_POP(STACK *stack)
{
    return *--stack->next;
}

#define STACK_PUSH WITH_TYPE(Stack_push)
static void STACK_PUSH(STACK *stack, STACK_TYPE n)
{
    *stack->next++ = n;
}

#undef WITH_TYPE
#undef STACK
#undef STACK_INIT
#undef STACK_EMPTY
#undef STACK_PEEK
#undef STACK_POP
#undef STACK_PUSH
#endif
