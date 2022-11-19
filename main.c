#include <stdio.h>
#include "sds.h"
#include "list.h"

void testList();
void testStr();

int main() {
    printf("Hello, World!\n");
    testStr();
    testList();

    return 0;
}

void testStr() {
    char *str = sdsnew("OK");
    printf("the formated str is:%s\n", str);
}

void testList() {
    list *alist = listCreate();
    for (int i = 0; i < 10; i++) {
        listAddNodeHead(alist, i);
    }

    listNode *node = listSearchKey(alist, 6);
    printf("current v:%d, prev:%d, next:%d\n", node->value,
           node->prev->value, node->next->value);

    listDelNode(alist, node);
    node = listSearchKey(alist, 6);
    if (node != NULL) {
        printf("eror to del the key:%d\n", 6);
    }

    listAddNodeHead(alist, 11);
    printf("the list head value is:%d\n", alist->head->value);

    node = listIndex(alist, 2);
    printf("the list index:%d  value is:%d\n", 2, node->value);

    listRelease(alist);
}
