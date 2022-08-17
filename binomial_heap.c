#include "common.h"
#include "element_types.h"

typedef struct HeapNode_st
{
    Key key;
    Object object;
    // height here is the height of the full binary tree plus one
    // Therefore, the number of nodes at a node plus all nodes in node->right is 2^height
    int height;
    struct HeapNode_st *left;
    struct HeapNode_st *right;
} HeapNode;

#define STACK_TYPE HeapNode *
#define STACK_MAX 200
#include "bounded_stack.h"
#undef STACK_MAX
#undef STACK_TYPE

static void HeapNode_init(HeapNode *n, Key key, Object object)
{
    n->key = key;
    n->object = object;
    n->height = 0;
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
    HeapNode *heap = NewHeapNode(NULL_KEY, NULL_OBJECT);
    heap->height = -1;
    return heap;
}

#define Heap_IS_EMPTY(heapnode) (heapnode->height == -1)

// Precondition: Heap is not empty.
static void Heap_find_min(HeapNode *heap, Key *key, Object *object)
{
    *key = heap->key;
    *object = heap->object;
    for (HeapNode *current = heap->left; current != NULL; current = current->left)
    {
        if (*key > current->key)
        {
            *key = current->key;
            *object = current->object;
        }
    }
}

// Note that this implementation of merge heaps cause Heap_insert to be O(log n) amortized instead of O(1).
// One possible way to achieve O(1) amortized insert would be to have a binomial heap where the tree heights increase instead of decrease
static HeapNode *merge_heaps(HeapNode *heap1, HeapNode *heap2)
{
    if (heap1 != NULL && Heap_IS_EMPTY(heap1))
    {
        HeapNode_free(heap1);
        heap1 = NULL;
    }

    if (heap2 != NULL && Heap_IS_EMPTY(heap2))
    {
        HeapNode_free(heap2);
        heap2 = NULL;
    }

    Stack stack, *stackp = &stack;
    Stack_init(stackp);

    while (heap1 != NULL && heap2 != NULL)
    {
        if (heap1->height > heap2->height)
        {
            Stack_push(stackp, heap1);
            heap1 = heap1->left;
        }
        else
        {
            Stack_push(stackp, heap2);
            heap2 = heap2->left;
        }
    }

    for (; heap1 != NULL; heap1 = heap1->left)
        Stack_push(stackp, heap1);
    for (; heap2 != NULL; heap2 = heap2->left)
        Stack_push(stackp, heap2);

    if (Stack_empty(stackp))
        return NULL;

    HeapNode *a = Stack_pop(stackp);
    while (!Stack_empty(stackp))
    {
        HeapNode *b = Stack_pop(stackp);

        // We want to do "a + b"
        if (a->height < b->height)
        {
            b->left = a;
            a = b;
        }
        else if (a->height == b->height)
        {
            if (a->key < b->key)
            {
                b->left = a->right;
                a->right = b;
            }
            else
            {
                b->left = a->right;
                a->left = b->right;
                b->right = a;
                a = b;
            }
            a->height++;
        }
        else // a->height > b->height
        {
            assert(a->height == b->height + 1);
            b->left = a->left;
            a->left = b;
        }
    }

    return a;
}

static void Heap_insert(HeapNode *heap, Key key, Object object)
{
    HeapNode *to_insert = NewHeapNode(key, object);
    HeapNode *heap_copy = NewHeapNode(NULL_KEY, NULL_OBJECT);
    *heap_copy = *heap;
    HeapNode *merged = merge_heaps(heap_copy, to_insert);
    *heap = *merged;
    HeapNode_free(merged);
}

// Precondition: Heap is non-empty.
static void Heap_delete_min(HeapNode *heap, Key *key, Object *object)
{
    HeapNode *min_node = heap, *min_node_parent = NULL;
    for (HeapNode *current = heap->left, *parent = heap; current != NULL; parent = current, current = current->left)
    {
        if (current->key < min_node->key)
        {
            min_node = current;
            min_node_parent = parent;
        }
    }

    *key = min_node->key;
    *object = min_node->object;
    HeapNode *merged;
    if (min_node_parent == NULL)
    {
        assert(min_node == heap);
        merged = merge_heaps(min_node->left, min_node->right);
    }
    else
    {
        min_node_parent->left = min_node->left;
        HeapNode *heap_copy = NewHeapNode(NULL_KEY, NULL_OBJECT);
        *heap_copy = *heap;
        merged = merge_heaps(heap_copy, min_node->right);
    }
    *heap = *merged;
    HeapNode_free(merged);
}
