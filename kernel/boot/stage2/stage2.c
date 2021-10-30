#include "header.h"
#include "print.h"
#include "memory.h"
#include "disk.h"

void a(void) {
}

void entry(void) {
    clrscr();
    dispinthex(0xCEFE32DA, 0x0C, 7, 0, true);
    // readsector(0, 0, 0x1000, 0x0000);
    // for (short i = 0 ; i < 10 ; i ++) {
    //     for (short j = 0 ; j < 16 ; j ++) {
    //         dispbytehex(getbyte(0x1000, 0x0000 + i * 16 + j), 0x0C, j * 3, i, false);
    //     }
    // }
    //dispstr("hello, os world!", 0x0C, 0, 0);
    a();
}