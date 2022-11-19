#include <stdio.h>
#include "sds.h"

int main() {
    char * str = sdsnew("OK");
    printf("Hello, World!\n");
    printf("the formated str is:%s\n", str);
    return 0;
}
