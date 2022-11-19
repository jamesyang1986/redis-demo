//
// Created by james on 2022/11/19.
//

#ifndef REDIS_DEMO_LIST_H
#define REDIS_DEMO_LIST_H

typedef struct listNode {
  struct listNode *prev;
  struct listNode *next;
  void *value;
} listNode;


typedef struct list {
    listNode *head;
    listNode *tail;
    int len;
} list;


/* Prototypes */
list *listCreate(void);
void listRelease(list *list);
list *listAddNodeHead(list *list, void *value);
list *listAddNodeTail(list *list, void *value);
void listDelNode(list *list, listNode *node);
listNode *listSearchKey(list *list, void *key);
listNode *listIndex(list *list, int index);

#endif //REDIS_DEMO_LIST_H
