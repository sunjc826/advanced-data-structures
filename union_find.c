#include "common.h"
#include "element_types.h"

typedef struct UfNode_st
{
    Object object;
    int rank;
    struct UfNode_st *up;
} UfNode;

UfNode *NewUfNode(Object object)
{
    UfNode *n = malloc(sizeof(UfNode));
    n->object = object;
    n->rank = 0;
    n->up = NULL;
}

#define insert(object) (NewUfNode(object))

#define UfNode_IS_ROOT(node) ((node)->up == NULL)

#define STACK_TYPE UfNode *
#define STACK_MAX 100
#include "bounded_stack.h"
#undef STACK_MAX
#undef STACK_TYPE

UfNode *UfNode_find_class(UfNode *n)
{
    Stack stack, *stackp = &stack;
    Stack_init(stackp);

    for (; !UfNode_IS_ROOT(n); n = n->up)
        Stack_push(stackp, n);

    while (!Stack_empty(stackp))
        Stack_pop(stackp)->up = n;

    return n;
}

// This is similar to the version used in the book.
// Memory usage is O(1).
UfNode *UfNode_find_class_v2(UfNode *n)
{
    UfNode *current;
    for (current = n; !UfNode_IS_ROOT(current); current = current->up)
        ;

    UfNode *tmp;
    for (tmp = n->up; n != current; n = tmp, tmp = tmp->up)
        n->up = current;

    return current;
}

bool same_class(UfNode *n1, UfNode *n2)
{
    return UfNode_find_class(n1) == UfNode_find_class(n2);
}

void join(UfNode *n1, UfNode *n2)
{
    UfNode *class1 = UfNode_find_class(n1);
    UfNode *class2 = UfNode_find_class(n2);
    if (class1->rank < class2->rank)
        class1->up = class2;
    else if (class1->rank > class2->rank)
        class2->up = class1;
    else
    {
        class2->up = class1;
        class1->rank++;
    }
}

// This is similar to the version used in the book.
// It is probably faster, but does not lend well to reusing the find_class function.
void join_v2(UfNode *n1, UfNode *n2)
{
    UfNode *class1, *class2, *new_root;
    for (class1 = n1; !UfNode_IS_ROOT(class1); class1 = class1->up)
        ;
    for (class2 = n2; !UfNode_IS_ROOT(class2); class2 = class2->up)
        ;
    if (class1->rank < class2->rank)
    {
        new_root = class2;
        class2->up = new_root;
    }
    else if (class1->rank > class2->rank)
    {
        new_root = class1;
        class2->up = new_root;
    }
    else
    {
        new_root = class1;
        class2->up = new_root;
        new_root->rank++;
    }

    UfNode *tmp;
    for (tmp = n1->up; n1 != new_root; n1 = tmp, tmp = tmp->up)
        n1->up = new_root;
    for (tmp = n2->up; n2 != new_root; n2 = tmp, tmp = tmp->up)
        n2->up = new_root;
}
