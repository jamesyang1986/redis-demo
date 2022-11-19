#include <stdio.h>
#include "sds.h"
#include "list.h"
#include "dict.h"

void testList();
void testStr();

void testDict();

int main() {
    printf("Hello, World!\n");
    testStr();

    testList();

    testDict();

    return 0;
}

void testDict() {
    dict *adict = dictCreate();
    dictAdd(adict, "aaa", "cc");
    dictEntry  *entry = dictFind(adict, "aaa");
    printf("test add, original value:%s, real value is:%s\n", "cc", entry->val);

    dictReplace(adict, "aaa", "23452345");
    entry = dictFind(adict, "aaa");
    printf("test replace, original value:%s, real value is:%s\n", "cc", entry->val);

    dictDelete(adict, "aaa");
    entry = dictFind(adict, "aaa");
    if(entry == NULL){
        printf("test del key:%s success!", "aaa");
    }
}

void testStr() {
    char *str = sdsnew("OK");
    printf("the sds str is:%s\n", str);
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
