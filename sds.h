//
// Created by james on 2022/11/19.
//

#ifndef REDIS_DEMO_SDS_H
#define REDIS_DEMO_SDS_H

#include <sys/types.h>

typedef char *sds;
typedef struct sdshdr {
    long len;
    long free;
    char buf[0];
};

sds sdsnewlen(const void *init, size_t initlen);
sds sdsnew(const char *init);

#endif //REDIS_DEMO_SDS_H
