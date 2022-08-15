#include <common.h>
#include <element_types.h>
typedef struct HeapNode_st
{
    Key key;
    Object object;
    int rank;
    struct HeapNode_st *left;
    struct HeapNode_st *right;
} HeapNode;

#define RANK(heapnode) (heapnode == NULL ? 0 : heapnode->rank)

static void HeapNode_init(HeapNode *n, Key key, Object object)
{
    n->key = key;
    n->object = object;
    n->rank = 1;
    n->left = NULL;
    n->right = NULL;
}

static HeapNode *NewHeapNode(Key key, Object object)
{
    HeapNode *n = malloc(sizeof(HeapNode));
    HeapNode_init(n, key, object);
    return n;
}

static void HeapNode_free(HeapNode *n)
{
    free(n);
}

static HeapNode *NewHeap(void)
{
    HeapNode *n = NewHeapNode(NULL_KEY, NULL_OBJECT);
    n->rank = 0;
    return n;
}

#define Heap_IS_EMPTY(heapnode) (((HeapNode *)heapnode)->rank == 0)

// Precondition: heap is non-empty.
static void Heap_find_min(HeapNode *heap, Key *key, Object *object)
{
    *key = heap->key;
    *object = heap->object;
}

#define STACK_TYPE HeapNode *
#define STACK_MAX 200
#include "bounded_stack.h"
#undef STACK_MAX
#undef STACK_TYPE

static void HeapNode_update_rank(HeapNode *n)
{
    if (n->left == NULL || n->right == NULL)
        n->rank = 1;
    else
        n->rank = MIN(n->left->rank, n->right->rank);
}

static void rebalance_up_ancestor_chain(Stack *ancestor_chain)
{
    while (!Stack_empty(ancestor_chain))
    {
        HeapNode *ancestor = Stack_pop(ancestor_chain);
        int old_rank = ancestor->rank;
        HeapNode_update_rank(ancestor);

        if (RANK(ancestor->left) < RANK(ancestor->right))
            SWAP(HeapNode *, ancestor->left, ancestor->right);

        if (ancestor->rank == old_rank)
            break;
    }
}

static void rebalance_all(Stack *ancestor_chain)
{
    while (!Stack_empty(ancestor_chain))
    {
        HeapNode *ancestor = Stack_pop(ancestor_chain);
        int old_rank = ancestor->rank;
        HeapNode_update_rank(ancestor);

        if (RANK(ancestor->left) < RANK(ancestor->right))
            SWAP(HeapNode *, ancestor->left, ancestor->right);
    }
}

static void Heap_insert(HeapNode *heap, Key key, Object object)
{
    if (Heap_IS_EMPTY(heap))
    {
        heap->key = key;
        heap->object = object;
        heap->rank = 1;
        return;
    }

    HeapNode *to_insert = NewHeapNode(key, object);
    Stack ancestor_trace, *ancestor_tracep = &ancestor_trace;
    HeapNode *current;
    for (current = heap; current != NULL && key > current->key; current = current->right)
        Stack_push(ancestor_tracep, current);

    to_insert->left = current;
    HeapNode *parent = Stack_peek(ancestor_tracep);
    parent->right = to_insert;
    rebalance_up_ancestor_chain(ancestor_tracep);
}

static HeapNode *merge_heaps(HeapNode *heap1, HeapNode *heap2)
{
    HeapNode *sentinel = NewHeapNode(NULL_KEY, NULL_OBJECT);

    HeapNode *right_most = sentinel;
    while (heap1 != NULL && heap2 != NULL)
    {
        if (heap1->key < heap2->key)
        {
            right_most->right = heap1;
            right_most = heap1;
            heap1 = heap1->right;
        }
        else
        {
            right_most->right = heap2;
            right_most = heap2;
            heap2 = heap2->right;
        }
    }

    if (heap1 != NULL)
    {
        right_most->right = heap1;
    }

    if (heap2 != NULL)
    {
        right_most->right = heap2;
    }

    HeapNode *result = sentinel->right;
    HeapNode_free(sentinel);
    Stack stack, *stackp = &stack;
    Stack_init(stackp);
    for (HeapNode *current = result; current != NULL; current = current->right)
        Stack_push(stackp, current);
    rebalance_all(stackp);
    return result;
}

// Precondition: heap is non-empty.
static void Heap_delete_min(HeapNode *heap, Key *key, Object *object)
{
    *key = heap->key;
    *object = heap->object;

    HeapNode *merged = merge_heaps(heap->left, heap->right);
    *heap = *merged;
    HeapNode_free(merged);
}
