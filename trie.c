#include "common.h"
#include "element_types.h"

typedef struct TrieNode_st
{
    union
    {
        struct TrieNode_st *node;
        Object object;
    } children[256];
} TrieNode;

#define STACK_TYPE TrieNode *
#define STACK_MAX 200 // assume no string is longer than 200 characters
#include "bounded_stack.h"
#undef STACK_MAX
#undef STACK_TYPE

#define CHILD(n, i) ((n)->children[(int)(i)].node)
#define OBJECT(n) ((n)->children['\0'].object)

void TrieNode_init(TrieNode *n)
{
    OBJECT(n) = NULL_OBJECT;
    for (int i = 1; i < 256; i++)
        CHILD(n, i) = NULL;
}

TrieNode *NewTrieNode(void)
{
    TrieNode *n = malloc(sizeof(TrieNode));
    TrieNode_init(n);
}

TrieNode *NewTrie(void)
{
    return NewTrieNode();
}

void TrieNode_free(TrieNode *n)
{
    free(n);
}

// Returns NULL_OBJECT if not found.
Object Trie_find(TrieNode *root, char *s)
{
    unsigned char *u = (unsigned char *)s;
    TrieNode *current = root;
    for (; *u != '\0'; u++)
    {
        if (CHILD(current, u) == NULL)
            return NULL_OBJECT;
        current = CHILD(current, u);
    }

    return OBJECT(current);
}

// Returns false if insertion failed.
bool Trie_insert(TrieNode *root, char *s, Object object)
{
    assert(object != NULL_OBJECT);
    unsigned char *u = (unsigned char *)s;
    TrieNode *current = root;
    for (; *u != '\0'; u++)
    {
        if (CHILD(current, u) == NULL)
            CHILD(current, u) = NewTrieNode();
        current = CHILD(current, u);
    }

    if (OBJECT(current) != NULL_OBJECT)
        return false;

    OBJECT(current) = object;
    return true;
}

// Returns NULL_OBJECT if not found.
Object Trie_delete(TrieNode *root, char *s)
{
    Object result;
    unsigned char *u = (unsigned char *)s;
    TrieNode *current = root;
    Stack stack, *stackp = &stack;
    Stack_init(stackp);
    int i;
    bool still_in_use;
    for (; *u != '\0'; u++)
    {
        if (CHILD(current, u) == NULL)
            return NULL_OBJECT;
        current = CHILD(current, u);
        Stack_push(stackp, current);
    }

    result = OBJECT(current);
    OBJECT(current) = NULL_OBJECT;
    while (!Stack_empty(stackp))
    {
        current = Stack_pop(stackp);
        CHILD(current, u) = NULL; // technically we are also setting current->children[0].object to NULL, but that's okay
        still_in_use = true;
        for (i = 1; i < 256; i++)
        {
            if (CHILD(current, i) != NULL)
            {
                still_in_use = false;
                break;
            }
        }

        if (!still_in_use)
            break;
        TrieNode_free(current);
        u--;
    }

    return result;
}
