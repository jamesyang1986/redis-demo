//
// Created by james on 2022/11/19.
//

#include "sds.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


sds sdsnewlen(const void *init, size_t initlen) {
    struct sdshdr *sh;
    sh = malloc(sizeof(struct sdshdr) + initlen + 1);
    if (sh == NULL) return NULL;

    sh->len = initlen;
    sh->free = 0;
    if (initlen) {
        if (init) {
            memcpy(sh->buf, init, initlen);
        } else {
            memset(sh->buf, 0, initlen);
        }
    }
    sh->buf[initlen] = '\0';

    return (char *)sh->buf;
}

sds sdsnew(const char *init) {
    size_t slen = (init == NULL) ? 0 : sizeof(init);
    return sdsnewlen(init, slen);
}
