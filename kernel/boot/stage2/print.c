// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong Beanflame
 *
 * kernel/boot/stage2/print.c
 *
 * Display functions are implemented here
 */



#include "print.h"
#include "string.h"

#define PRINT_COLOR 0x0F

short print_x = 0;
short print_y = 0;

void printchar(char ch) {
    dispchar(ch, PRINT_COLOR, print_x ++, print_y);
}

void printstr(const char *str) {
    while (*str != 0) {
        if (*str == '\n') {
            printreturn();
            str ++;
        }
        else {
            printchar(*(str ++));
        }
    }
}

void printreturn(void) {
    print_x = 0;
    print_y ++;
}

void printbyte(byte num, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            printchar('-');
            num = -num;
        }
    }
    char str[5];
    short top = 0;
    while (num) {
        str[++ top] = num % 10 + '0';
        num /= 10;
    }
    if (!top)
        str[++ top] = '0';
    while (top) {
        printchar(str[top --]);
    }
}

void printbytehex(byte num, bool disp_0x) {
    if (disp_0x) {
        printchar('0');
        printchar('x');
    }
    char str[5];
    short top = 0;
    while (num) {
        str[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 2 - top;
    while (cnt --)
        printchar('0');
    while (top) {
        printchar(str[top --]);
    }
}

void printshort(word num, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            printchar('-');
            num = -num;
        }
    }
    char str[6];
    short top = 0;
    while (num) {
        str[++ top] = num % 10 + '0';
        num /= 10;
    }
    if (!top)
        str[++ top] = '0';
    while (top) {
        printchar(str[top --]);
    }
}

void printshorthex(word num, bool disp_0x) {
    if (disp_0x) {
        printchar('0');
        printchar('x');
    }
    char str[7];
    short top = 0;
    while (num) {
        str[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 4 - top;
    while (cnt --)
        printchar('0');
    while (top) {
        printchar(str[top --]);
    }
}

void printint(dword num, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            printchar('-');
            num = -num;
        }
    }
    char str[25];
    short top = 0;
    while (num) {
        str[++ top] = num % 10 + '0';
        num /= 10;
    }
    if (!top)
        str[++ top] = '0';
    while (top) {
        printchar(str[top --]);
    }
}
void printinthex(dword num, bool disp_0x) {
    if (disp_0x) {
        printchar('0');
        printchar('x');
    }
    char str[13];
    short top = 0;
    while (num) {
        str[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 8 - top;
    while (cnt --)
        printchar('0');
    while (top) {
        printchar(str[top --]);
    }
}

void changepos(word x, word y) {
    print_x = x;
    print_y = y;
}

word getposx(void) {
    return print_x;
}

word getposy(void) {
    return print_y;
}

void dispchar(char ch, byte color, byte x, byte y) {
    stgs(0xB800);
    wrgs8(ch, (y * 80 + x) * 2);
    wrgs8(color, (y * 80 + x) * 2 + 1);
}

void _clrscr(void);
void clrscr(void) {
    _clrscr();
    ed_callasm();
}

void dispstr(const char *str, byte color, byte x, byte y) {
    while (*str != 0)
        dispchar(*(str ++), color, x ++, y);
}

void dispbyte(byte num, byte color, byte x, byte y, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            dispchar('-', color, x ++, y);
            num = -num;
        }
    }
    char S[5];
    short top = 0;
    while (num) {
        S[++ top] = num % 10;
        num /= 10;
    }
    if (!top)
        S[++ top] = 0;
    while (top) {
        dispchar(S[top --] + '0', color, x ++, y);
    }
}

void dispbytehex(byte num, byte color, byte x, byte y, bool disp_0x) {
    if (disp_0x) {
        dispchar('0', color, x ++, y);
        dispchar('x', color, x ++, y);
    }

    char S[5];
    short top = 0;
    while (num) {
        S[++top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 2 - top;
    while (cnt --)
        dispchar('0', color, x ++, y);
    while (top) {
        dispchar(S[top --], color, x ++, y);
    }
}

void dispshort(word num, byte color, byte x, byte y, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            dispchar('-', color, x ++, y);
            num = -num;
        }
    }
    char S[7];
    short top = 0;
    while (num) {
        S[++ top] = num % 10;
        num /= 10;
    }
    if (!top)
        S[++ top] = 0;
    while (top) {
        dispchar(S[top --] + '0', color, x ++, y);
    }
}

void dispshorthex(word num, byte color, byte x, byte y, bool disp_0x) {
    if (disp_0x) {
        dispchar('0', color, x ++, y);
        dispchar('x', color, x ++, y);
    }

    char S[7];
    short top = 0;
    while (num) {
        S[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 4 - top;
    while (cnt --)
        dispchar('0', color, x ++, y);
    while (top) {
        dispchar(S[top --], color, x ++, y);
    }
}

void dispint(dword num, byte color, byte x, byte y, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            dispchar('-', color, x ++, y);
            num = -num;
        }
    }
    char S[14];
    short top = 0;
    while (num) {
        S[++ top] = num % 10;
        num /= 10;
    }
    if (!top)
        S[++ top] = 0;
    while (top) {
        dispchar(S[top --] + '0', color, x ++, y);
    }
}

void dispinthex(dword num, byte color, byte x, byte y, bool disp_0x) {
    if (disp_0x) {
        dispchar('0', color, x ++, y);
        dispchar('x', color, x ++, y);
    }

    char S[20];
    short top = 0;
    while (num) {
        S[++top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 8 - top;
    while (cnt --)
        dispchar('0', color, x ++, y);
    while (top) {
        dispchar(S[top --], color, x ++, y);
    }
}