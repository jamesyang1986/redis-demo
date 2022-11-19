//
// Created by james on 2022/11/19.
//
#include <stdlib.h>
#include "list.h"

list *listCreate(void) {
    struct list *list;
    if ((list = malloc(sizeof(struct list))) == NULL)
        return NULL;

    list->head = list->tail = NULL;
    list->len = 0;
    return list;
}

void listRelease(list *list) {
    if (list == NULL)
        return;

    int len;
    listNode *current, *next;

    current = list->head;
    len = list->len;
    while (len--) {
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

list *listAddNodeHead(list *list, void *value) {
    listNode *node = malloc(sizeof(listNode *));
    if (list == NULL || node == NULL) {
        return list;
    }

    node->next = node->prev = NULL;
    node->value = value;

    if (list->len == 0) {
        list->head = node;
        list->tail = node;
    } else {
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
    }
    list->len++;

    return list;
}

list *listAddNodeTail(list *list, void *value) {
    listNode *node = malloc(sizeof(listNode *));
    if (list == NULL || node == NULL) {
        return list;
    }

    node->next = node->prev = NULL;
    node->value = value;

    if (list->len == 0) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }

    list->len++;

    return list;
}

void listDelNode(list *list, listNode *node) {
    if (list == NULL || node == NULL)
        return;
    listNode *current;
    current = list->head;

    while (current != NULL) {
        if (current->value == node->value) {
            if (current->prev) {
                current->prev->next = current->next;
            }
            free(current);
            list->len--;
            break;
        } else {
            current = current->next;
        }
    }



}

listNode *listSearchKey(list *list, void *key) {
    if (list == NULL)
        return NULL;
    listNode *current;
    current = list->head;

    while (current != NULL) {
        if (current->value == key) {
            return current;
        } else {
            current = current->next;
        }
    }

    return NULL;
}

listNode *listIndex(list *list, int index) {
    if (list == NULL)
        return NULL;
    listNode *current = list->head;

    while (index-- && (current != NULL)) {
        current = current->next;
    }

    if (current)
        return current;

    return NULL;

}