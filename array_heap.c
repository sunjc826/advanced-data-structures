// pg 218
#include "common.h"
#include "element_types.h"

typedef struct
{
    Key key;
    Object object;
} HeapElement;

typedef struct
{
    HeapElement *arr;
    int max_size;
    int current_size;
} Heap; // min heap

void Heap_init(Heap *heap, int max_size)
{
    heap->arr = malloc(sizeof(HeapElement) * max_size);
    heap->max_size = max_size;
    heap->current_size = 0;
}

void Heap_free_members(Heap heap)
{
    free(heap.arr);
}

void Heap_free(Heap *heap)
{
    Heap_free_members(*heap);
    free(heap);
}

#define Heap_IS_EMPTY(heap) (((Heap *)heap)->current_size == 0)
#define LEFT(i) (2 * i + 1)
#define RIGHT(i) (2 * i + 2)
#define PARENT(i) ((i - 1) / 2)

bool Heap_is_full(Heap *heap)
{
    return heap->current_size == heap->max_size;
}

// Precondition: Heap is non-empty
void Heap_find_min(Heap *heap, Key *key, Object *object)
{
    *key = heap->arr->key;
    *object = heap->arr->object;
}

// Precondition: 0 <= index < current_size
void Heap_bubble_up(Heap *heap, int index)
{
    HeapElement to_insert = heap->arr[index];
    int insertion_pos;
    for (insertion_pos = index; insertion_pos >= 0 && heap->arr[PARENT(insertion_pos)].key > heap->arr[insertion_pos].key; index = PARENT(insertion_pos))
        heap->arr[insertion_pos] = heap->arr[PARENT(insertion_pos)];
    heap->arr[insertion_pos] = to_insert;
}

void Heap_bubble_down(Heap *heap, int index)
{
    HeapElement to_insert = heap->arr[index];
    int insertion_pos, left, right, smaller_index;
    for (insertion_pos = index; LEFT(insertion_pos) < heap->current_size; insertion_pos = smaller_index)
    {
        left = LEFT(insertion_pos);
        right = RIGHT(insertion_pos);
        smaller_index = left;
        if (right < heap->current_size && heap->arr[right].key < heap->arr[left].key)
            smaller_index = right;

        if (to_insert.key <= heap->arr[smaller_index].key)
            break;

        heap->arr[insertion_pos] = heap->arr[smaller_index];
    }

    heap->arr[insertion_pos] = to_insert;
}

// Return Value: Returns whether insert is successful.
bool Heap_insert(Heap *heap, Key key, Object object)
{
    if (Heap_is_full(heap))
        return false;

    heap->arr[heap->current_size].key = key, heap->arr[heap->current_size++].object = object;
    Heap_bubble_up(heap, heap->current_size);
    return true;
}

// Precondition: Heap is non-empty
void Heap_delete(Heap *heap, Key *key, Object *object)
{
    *key = heap->arr[0].key;
    *object = heap->arr[0].object;
    HeapElement to_insert = heap->arr[--heap->current_size];
    if (heap->current_size == 0)
        return;
    // bottom-up heapsort
    int insertion_index, left, right, smaller_index;
    for (insertion_index = 0; LEFT(insertion_index) < heap->current_size; insertion_index = smaller_index)
    {
        left = LEFT(insertion_index);
        right = RIGHT(insertion_index);
        smaller_index = left;
        if (right < heap->current_size && heap->arr[left].key < heap->arr[right].key)
            smaller_index = left;
        heap->arr[insertion_index] = heap->arr[smaller_index];
    }

    heap->arr[insertion_index] = to_insert;
    Heap_bubble_up(heap, insertion_index);
}
