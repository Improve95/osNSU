#ifndef CACHELIST_H
#define CACHELIST_H


struct node_cache_date {

    char *data;
    int lengthData;

    struct node_cache_date *next;

} typedef NodeCacheData;

struct list_cache_data {
    NodeCacheData *head;
    NodeCacheData *tail;
} typedef ListCacheData;

ListCacheData *initDataCacheList();

void pushDataCacheBack(ListCacheData *head, char *data, int length);

NodeCacheData *getCacheNode(ListCacheData *list, int n);

void freeList(ListCacheData *list);

#endif //CACHELIST_H
