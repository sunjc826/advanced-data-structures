// pg 210
// This implementation does not use the suggestions (lazy delete) mentioned on pg 212.
// Because the underlying bbst implementation does not have linked list amongst its leaves.
#include "common.h"
#include "bbst.h"

#define Heap_IS_EMPTY(heap) (Tree_IS_EMPTY(((Heap *)heap)->tree))

typedef struct
{
    Key key;
    Object object;
} HeapElement;

typedef struct
{
    TreeNode *tree;
    HeapElement *min_element;
} Heap; // min heap

void Heap_init(Heap *heap)
{
    heap->tree = NewTree();
    heap->min_element = NULL;
}

Heap *NewHeap()
{
    Heap *heap = malloc(sizeof(Heap));
    Heap_init(heap);
    return heap;
}

// Precondition: heap is non-empty.
void Heap_find_min(Heap *heap, Key *key, Object *object)
{
    *key = heap->min_element->key;
    *object = heap->min_element->object;
}

// Note: Does not accept duplicate keys, due to the underlying BBST implementation
// Return Value: Returns whether insertion is successful.
bool Heap_insert(Heap *heap, Key key, Object object)
{
    HeapElement *el = malloc(sizeof(HeapElement));
    el->key = key;
    el->object = object;
    if (!Tree_insert(heap->tree, key, el))
        return false;
    if (key < heap->min_element->key)
        heap->min_element = el;
    return true;
}

// Precondition: heap is non-empty
void Heap_delete_min(Heap *heap, Key *key, Object *object)
{
    Object object = Tree_delete(heap->tree, heap->min_element->key);
    if (Tree_IS_EMPTY(heap->tree))
        heap->min_element = NULL;
    else
        heap->min_element = Tree_find_min(heap->tree);
    return object;
}
