#include "common.h"
#include "element_types.h"
typedef size_t (*HashFunction)(Key key, void *hash_fn_param);

typedef struct ListNode_st
{
    Key key;
    Object object;
    struct ListNode_st *next;
} ListNode;

ListNode *NewListNode(Key key, Object object)
{
    ListNode *n = malloc(sizeof(ListNode));
    n->key = key;
    n->object = object;
    return n;
}

typedef struct
{
    ListNode **table; // We can consider adding a sentinel node to each bucket in the table.
    size_t buckets;
    HashFunction fn;
    void *hash_fn_param;
} HashTable;

#define HASH(ht, key) (ht->fn((key), ht->hash_fn_param))

// find, insert, delete
void HashTable_init(HashTable *ht, size_t buckets, HashFunction fn, void *hash_fn_param)
{
    ht->table = malloc(sizeof(ListNode *) * buckets);
    for (int i = 0; i < buckets; i++)
        ht->table[i] = NULL;
    ht->buckets = buckets;
    ht->fn = fn;
    ht->hash_fn_param = hash_fn_param;
}

#define P 46337 // technically, we should calculate sqrt(SIZE_MAX)
typedef struct
{
    size_t a;
    size_t b;
    size_t buckets;
} HashFnParam;

size_t universal_hash_fn(Key key, HashFnParam *param)
{
    return (param->a * key + param->b) % P % param->buckets;
}

void HashTable_init_v2(HashTable *ht, size_t buckets)
{
    HashFnParam *param = malloc(sizeof(HashFnParam));
    param->a = rand() % P;
    param->b = rand() % P;

    HashTable_init(ht, buckets, universal_hash_fn, param);
}

Object HashTable_find(HashTable *ht, Key key)
{
    size_t hash = HASH(ht, key);
    for (ListNode *n = ht->table[hash]; n != NULL; n = n->next)
    {
        if (n->key == key)
            return n->object;
    }

    return NULL_OBJECT;
}

void HashTable_insert(HashTable *ht, Key key, Object object)
{
    size_t hash = HASH(ht, key);
    ListNode *head = ht->table[hash];
    ListNode *new_node = NewListNode(key, object);
    new_node->next = head;
    ht->table[hash] = new_node;
}

Object HashTable_delete(HashTable *ht, Key key)
{
    size_t hash = HASH(ht, key);
    ListNode *n = ht->table[hash];
    if (n == NULL)
        return NULL_OBJECT;

    if (n->key == key)
    {
        ht->table[hash] = n->next;
        return n->object;
    }

    for (; n->next != NULL && n->next->key != key; n = n->next)
        ;

    if (n->next == NULL)
        return NULL_OBJECT;

    Object result = n->next->object;
    n->next = n->next->next;
    return result;
}
