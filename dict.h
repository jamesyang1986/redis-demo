//
// Created by james on 2022/11/19.
//

#ifndef REDIS_DEMO_DICT_H
#define REDIS_DEMO_DICT_H

typedef struct dictEntry {
    void *key;
    void *val;
    struct dictEntry *next;
} dictEntry;

typedef struct dict {
    dictEntry **table;
    unsigned int size;
    unsigned int sizemask;
    unsigned int used;
} dict;


/* API */
dict *dictCreate();
int dictExpand(dict *ht, unsigned int size);
int dictAdd(dict *ht, void *key, void *val);
int dictReplace(dict *ht, void *key, void *val);
int dictDelete(dict *ht, const void *key);
void dictRelease(dict *ht);
dictEntry * dictFind(dict *ht, const void *key);
int dictResize(dict *ht);

#endif //REDIS_DEMO_DICT_H
