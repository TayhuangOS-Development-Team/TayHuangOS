#include <syscall/syscall.h>

#define NULL ((void*)0)

void entry(void) {
    eggs();
    while (1);
}