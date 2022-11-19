//
// Created by james on 2022/11/19.
//

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "dict.h"
#include "stdlib.h"

#define DICT_OK 0
#define DICT_ERR 1

#define DICT_HT_INITIAL_SIZE  16

#define DICT_DEFAULT_LOAD_FACTOR 0.75


dict *_init_dict(dict *adict);

void _initDict(dict *adict);

unsigned int dictHashKey(dict *ht, void *key);

int _dictKeyIndex(dict *pDict, void *pVoid);

void dictSetHashKey(dict *pDict, dictEntry *pEntry, void *pVoid);

/* ---------------------------- Utility funcitons --------------------------- */

static void _dictPanic(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "\nDICT LIBRARY PANIC: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n\n");
    va_end(ap);
}


dict *dictCreate() {
    dict *adict;
    adict = malloc(sizeof(*adict));
    if (adict == NULL) {
        _dictPanic("dict allocate oom.");
        return NULL;
    }

    _initDict(adict);
    return adict;

}

void _initDict(dict *adict) {
    adict->size = 0;
    adict->sizemask = 0;
    adict->table = NULL;
    adict->used = 0;
}

static int _dictExpandIfNeeded(dict *ht) {
    /* If the hash table is empty expand it to the intial size,
     * if the table is "full" dobule its size. */
    if (ht->size == 0)
        return dictExpand(ht, DICT_HT_INITIAL_SIZE);
    if (ht->used == ht->size)
        return dictExpand(ht, ht->size * 2);
    return DICT_OK;
}

/* Our hash table capability is a power of two */
static unsigned int _dictNextPower(unsigned int size) {
    unsigned int i = DICT_HT_INITIAL_SIZE;

    if (size >= 2147483648U)
        return 2147483648U;
    while (1) {
        if (i >= size)
            return i;
        i *= 2;
    }
}

int dictExpand(dict *ht, unsigned int size) {
    /* the size is invalid if it is smaller than the number of
     * elements already inside the hashtable */
    if (ht->used > size)
        return DICT_ERR;

    unsigned int realsize = _dictNextPower(size), i;

    dict n; /* the new hashtable */
    _initDict(&n);
    n.size = realsize;
    n.sizemask = realsize - 1;
    n.table = malloc(realsize * sizeof(dictEntry *));

    /* Initialize all the pointers to NULL */
    memset(n.table, 0, realsize * sizeof(dictEntry *));

    /* Copy all the elements from the old to the new table:
     * note that if the old hash table is empty ht->size is zero,
     * so dictExpand just creates an hash table. */
    n.used = ht->used;
    for (i = 0; i < ht->size && ht->used > 0; i++) {
        dictEntry *he, *nextHe;

        if (ht->table[i] == NULL) continue;

        /* For each hash entry on this slot... */
        he = ht->table[i];
        while (he) {
            unsigned int h;

            nextHe = he->next;
            /* Get the new element index */
            h = dictHashKey(ht, he->key) & n.sizemask;
            he->next = n.table[h];
            n.table[h] = he;
            ht->used--;
            /* Pass to the next element */
            he = nextHe;
        }
    }
    free(ht->table);

    /* Remap the new hashtable in the old */
    *ht = n;
    return DICT_OK;
}

unsigned int dictHashKey(dict *ht, void *key) {
    unsigned int hash = 5381;
    char *buf = (char *) key;
    int len = strlen(key);
    while (len--)
        hash = ((hash << 5) + hash) + (*buf++); /* hash * 33 + c */
    return hash;
}

int dictAdd(dict *ht, void *key, void *val) {
    int index;
    dictEntry *entry;

    /* Get the index of the new element, or -1 if
     * the element already exists. */
    if ((index = _dictKeyIndex(ht, key)) == -1)
        return DICT_ERR;

    /* Allocates the memory and stores key */
    entry = malloc(sizeof(*entry));
    entry->next = ht->table[index];
    ht->table[index] = entry;

    entry->key = key;
    entry->val = val;
    ht->used++;
    return DICT_OK;

}


int _dictKeyIndex(dict *ht, void *key) {

    unsigned int h;
    dictEntry *he;

    /* Expand the hashtable if needed */
    if (_dictExpandIfNeeded(ht) == DICT_ERR)
        return -1;
    /* Compute the key hash value */
    h = dictHashKey(ht, key) & ht->sizemask;
    /* Search if this slot does not already contain the given key */
    he = ht->table[h];
    while (he) {
        if (strcmp(he->key, key) == 0)
            return -1;
        he = he->next;
    }
    return h;
}

int dictReplace(dict *ht, void *key, void *val) {
    dictEntry *entry;

    /* Try to add the element. If the key
     * does not exists dictAdd will suceed. */
    if (dictAdd(ht, key, val) == DICT_OK)
        return DICT_OK;
    /* It already exists, get the entry */
    entry = dictFind(ht, key);
    /* Free the old value and set the new one */

    entry->val = val;
    return DICT_OK;
}

int dictDelete(dict *ht, const void *key) {
    unsigned int h;
    dictEntry *he, *prevHe;

    if (ht->size == 0)
        return DICT_ERR;
    h = dictHashKey(ht, key) & ht->sizemask;
    he = ht->table[h];

    prevHe = NULL;
    while (he) {
        if (strcmp(he->key, key) == 0) {
            /* Unlink the element from the list */
            if (prevHe)
                prevHe->next = he->next;
            else
                ht->table[h] = he->next;

            free(he);
            ht->used--;
            return DICT_OK;
        }
        prevHe = he;
        he = he->next;
    }
    return DICT_ERR; /* not found */


}

void dictRelease(dict *ht) {
    unsigned int i;
    /* Free all the elements */
    for (i = 0; i < ht->size && ht->used > 0; i++) {
        dictEntry *he, *nextHe;

        if ((he = ht->table[i]) == NULL) continue;
        while (he) {
            nextHe = he->next;
            free(he);
            ht->used--;
            he = nextHe;
        }
    }
    /* Free the table and the allocated cache structure */
    free(ht->table);
}

dictEntry *dictFind(dict *ht, const void *key) {
    dictEntry *he;
    unsigned int h;

    if (ht->size == 0) return NULL;
    h = dictHashKey(ht, key) & ht->sizemask;
    he = ht->table[h];
    while (he) {
        if (strcmp(he->key, key) == 0)
            return he;
        he = he->next;
    }

    return NULL;
}

int dictResize(dict *ht) {
    int minimal = ht->used;

    if (minimal < DICT_HT_INITIAL_SIZE)
        minimal = DICT_HT_INITIAL_SIZE;
    return dictExpand(ht, minimal);
}



