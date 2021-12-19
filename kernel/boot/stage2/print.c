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
#include "drivers/drivers.h"
#include "drivers/devices.h"
#include "drivers/vedio/vedio_driver.h"

#define PRINT_COLOR 0x0F

PRIVATE short print_x = 0;
PRIVATE short print_y = 0;

PUBLIC void printChar(char ch) {
    dispChar(ch, PRINT_COLOR, print_x ++, print_y);
}

PUBLIC void printStr(const char *str) {
    while (*str != 0) {
        if (*str == '\n') {
            printReturn();
            str ++;
        }
        else {
            printChar(*(str ++));
        }
    }
}

PUBLIC void printReturn(void) {
    print_x = 0;
    print_y ++;
}

PUBLIC void printByte(byte num, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            printChar('-');
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
        printChar(str[top --]);
    }
}

PUBLIC void printByteHex(byte num, bool disp_0x) {
    if (disp_0x) {
        printChar('0');
        printChar('x');
    }
    char str[5];
    short top = 0;
    while (num) {
        str[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 2 - top;
    while (cnt --)
        printChar('0');
    while (top) {
        printChar(str[top --]);
    }
}

PUBLIC void printShort(word num, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            printChar('-');
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
        printChar(str[top --]);
    }
}

PUBLIC void printShortHex(word num, bool disp_0x) {
    if (disp_0x) {
        printChar('0');
        printChar('x');
    }
    char str[7];
    short top = 0;
    while (num) {
        str[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 4 - top;
    while (cnt --)
        printChar('0');
    while (top) {
        printChar(str[top --]);
    }
}

PUBLIC void printInt(dword num, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            printChar('-');
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
        printChar(str[top --]);
    }
}

PUBLIC void printIntHex(dword num, bool disp_0x) {
    if (disp_0x) {
        printChar('0');
        printChar('x');
    }
    char str[13];
    short top = 0;
    while (num) {
        str[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 8 - top;
    while (cnt --)
        printChar('0');
    while (top) {
        printChar(str[top --]);
    }
}

PUBLIC void changePos(word x, word y) {
    print_x = x;
    print_y = y;
}

PUBLIC word getPosX(void) {
    return print_x;
}

PUBLIC word getPosY(void) {
    return print_y;
}

vd_writebyte_ap_t write_pack;

PUBLIC void dispChar(char ch, byte color, byte x, byte y) {
    write_pack.posX = x;
    write_pack.posY = y;
    write_pack.ch = ch;
    write_pack.color = color;
    vedio_driver.pc_handle(&vedio_device, &vedio_driver, VD_CMD_WRITE_BYTE, &write_pack);
}

PUBLIC void clrscr(void) {
    vedio_driver.pc_handle(&vedio_device, &vedio_driver, VD_CMD_CLRSCR, NULL);
}

PUBLIC void dispStr(const char *str, byte color, byte x, byte y) {
    while (*str != 0)
        dispChar(*(str ++), color, x ++, y);
}

PUBLIC void dispByte(byte num, byte color, byte x, byte y, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            dispChar('-', color, x ++, y);
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
        dispChar(S[top --] + '0', color, x ++, y);
    }
}

PUBLIC void dispByteHex(byte num, byte color, byte x, byte y, bool disp_0x) {
    if (disp_0x) {
        dispChar('0', color, x ++, y);
        dispChar('x', color, x ++, y);
    }

    char S[5];
    short top = 0;
    while (num) {
        S[++top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 2 - top;
    while (cnt --)
        dispChar('0', color, x ++, y);
    while (top) {
        dispChar(S[top --], color, x ++, y);
    }
}

PUBLIC void dispShort(word num, byte color, byte x, byte y, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            dispChar('-', color, x ++, y);
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
        dispChar(S[top --] + '0', color, x ++, y);
    }
}

PUBLIC void dispShortHex(word num, byte color, byte x, byte y, bool disp_0x) {
    if (disp_0x) {
        dispChar('0', color, x ++, y);
        dispChar('x', color, x ++, y);
    }

    char S[7];
    short top = 0;
    while (num) {
        S[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 4 - top;
    while (cnt --)
        dispChar('0', color, x ++, y);
    while (top) {
        dispChar(S[top --], color, x ++, y);
    }
}

PUBLIC void dispInt(dword num, byte color, byte x, byte y, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            dispChar('-', color, x ++, y);
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
        dispChar(S[top --] + '0', color, x ++, y);
    }
}

PUBLIC void dispIntHex(dword num, byte color, byte x, byte y, bool disp_0x) {
    if (disp_0x) {
        dispChar('0', color, x ++, y);
        dispChar('x', color, x ++, y);
    }

    char S[20];
    short top = 0;
    while (num) {
        S[++top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 8 - top;
    while (cnt --)
        dispChar('0', color, x ++, y);
    while (top) {
        dispChar(S[top --], color, x ++, y);
    }
}