#include "common.h"
#include "element_types.h"
typedef struct HeapNode_st
{
    Key key;
    Object object;
    struct HeapNode_st *left;
    struct HeapNode_st *right;
} HeapNode;

#define Heap_IS_EMPTY(heap) (((HeapNode *)heap)->object = NULL_OBJECT)

void HeapNode_init(HeapNode *n, Key key, Object object)
{
    n->key = key;
    n->object = object;
    n->left = NULL;
    n->right = NULL;
}

HeapNode *NewHeapNode(Key key, Object object)
{
    HeapNode *n = malloc(sizeof(HeapNode));
    HeapNode_init(n, key, object);
    return n;
}

void HeapNode_free(HeapNode *n)
{
    free(n);
}

HeapNode *NewHeap(void)
{
    return NewHeapNode(NULL_KEY, NULL_OBJECT);
}

// Precondition: Heap is non-empty.
void Heap_find_min(HeapNode *heap, Key *key, Object *object)
{
    *key = heap->key;
    *object = heap->object;
}

void Heap_insert_v1(HeapNode *heap, Key key, Object object)
{
    if (Heap_IS_EMPTY(heap))
    {
        heap->key = key;
        heap->object = object;
        return;
    }
    HeapNode *current = heap;
    while (true)
    {
        bool go_left = COIN_FLIP();
        if (go_left)
        {
            if (current->left == NULL || current->left->key >= key)
            {
                HeapNode *new_node = NewHeapNode(key, object);
                new_node->left = current->left;
                current->left = new_node;
                return;
            }
            else
                current = current->left;
        }
        else
        {
            if (current->right == NULL || current->right->key >= key)
            {
                HeapNode *new_node = NewHeapNode(key, object);
                new_node->right = current->right;
                current->right = new_node;
                return;
            }
            else
                current = current->right;
        }
    }
}

// Precondition: n must not be NULL
void pushdown_insert(HeapNode *n, Key key, Object object)
{
    Key displaced_key = key;
    Object displaced_object = object;
    HeapNode *parent;
    while (n != NULL)
    {
        parent = n;
        SWAP(Key, displaced_key, n->key);
        SWAP(Object, displaced_object, n->object);
        n = COIN_FLIP() ? n->left : n->right;
    }

    if (parent->left == NULL)
        parent->left = NewHeapNode(displaced_key, displaced_object);
    else
        parent->right = NewHeapNode(displaced_key, displaced_object);
}

void Heap_insert_v2(HeapNode *heap, Key key, Object object)
{
    if (Heap_IS_EMPTY(heap))
    {
        heap->key = key;
        heap->object = object;
        return;
    }
    HeapNode *current = heap;
    HeapNode *parent;
    while (current != NULL)
    {
        parent = current;
        if (key <= current->key)
        {
            pushdown_insert(current, key, object);
            return;
        }
        current = COIN_FLIP() ? current->left : current->right;
    }

    // since heap is non-empty, parent cannot be NULL
    if (parent->left == NULL)
        parent->left = NewHeapNode(key, object);
    else
        parent->right = NewHeapNode(key, object);
}

// Precondition: Heap is non-empty.
void Heap_delete_min(HeapNode *heap, Key *key, Object *object)
{
    *key = heap->key;
    *object = heap->object;
    HeapNode *current = heap, *parent;
    while (current->left != NULL && current->right != NULL)
    {
        parent = current;
        HeapNode *smaller = (current->left->key < current->right->key) ? current->left : current->right;
        current->key = smaller->key;
        current->object = smaller->object;
        current = smaller;
    }

    HeapNode *new_child;
    if (current->left != NULL)
        new_child = current->left;
    else if (current->right != NULL)
        new_child = current->right;
    else
        new_child = NULL;

    if (current == parent->left)
        parent->left = new_child;
    else
        parent->right = new_child;
}
