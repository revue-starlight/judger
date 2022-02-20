//inject.c
#include <stdio.h>

int hookReplacementFunction() {
    printf("Calling replacement function!\n");
    return 3;
}
__attribute__((constructor))
static void ctor(void) {
    printf("Dylib constructor called!\n");
}