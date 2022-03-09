#define INFINITY 100000000
typedef void *item_t;
int lower = 0;
int upper = 0;
item_t queue[INFINITY];

int queue_empty()
{
    return lower == upper;
}

void enqueue(item_t item)
{
    queue[upper] = item;
    upper++;
}

item_t dequeue()
{
    item_t item = queue[lower];
    lower++;
    return item;
}
