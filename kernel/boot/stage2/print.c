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

PUBLIC void print_char(char ch) {
    disp_char(ch, PRINT_COLOR, print_x ++, print_y);
}

PUBLIC void print_string(const char *str) {
    while (*str != 0) {
        if (*str == '\n') {
            print_return();
            str ++;
        }
        else {
            print_char(*(str ++));
        }
    }
}

PUBLIC void print_return(void) {
    print_x = 0;
    print_y ++;
}

PUBLIC void print_byte(byte num, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            print_char('-');
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
        print_char(str[top --]);
    }
}

PUBLIC void print_byte_hex(byte num, bool disp_0x) {
    if (disp_0x) {
        print_char('0');
        print_char('x');
    }
    char str[5];
    short top = 0;
    while (num) {
        str[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 2 - top;
    while (cnt --)
        print_char('0');
    while (top) {
        print_char(str[top --]);
    }
}

PUBLIC void print_short(word num, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            print_char('-');
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
        print_char(str[top --]);
    }
}

PUBLIC void print_short_hex(word num, bool disp_0x) {
    if (disp_0x) {
        print_char('0');
        print_char('x');
    }
    char str[7];
    short top = 0;
    while (num) {
        str[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 4 - top;
    while (cnt --)
        print_char('0');
    while (top) {
        print_char(str[top --]);
    }
}

PUBLIC void print_int(dword num, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            print_char('-');
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
        print_char(str[top --]);
    }
}

PUBLIC void print_int_hex(dword num, bool disp_0x) {
    if (disp_0x) {
        print_char('0');
        print_char('x');
    }
    char str[13];
    short top = 0;
    while (num) {
        str[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 8 - top;
    while (cnt --)
        print_char('0');
    while (top) {
        print_char(str[top --]);
    }
}

PUBLIC void change_pos(word x, word y) {
    print_x = x;
    print_y = y;
}

PUBLIC word get_pos_x(void) {
    return print_x;
}

PUBLIC word get_pos_y(void) {
    return print_y;
}

PRIVATE APACK(vd, writebyte) write_pack;

PUBLIC void disp_char(char ch, byte color, byte x, byte y) {
    write_pack.pos_x = x;
    write_pack.pos_y = y;
    write_pack.ch = ch;
    write_pack.color = color;
    ((pdriver)vedio_device.driver)->pc_handle(&vedio_device, vedio_device.driver, VD_CMD_WRITE_BYTE, &write_pack);
}

PUBLIC void clrscr(void) {
    ((pdriver)vedio_device.driver).pc_handle(&vedio_device, vedio_device.driver, VD_CMD_CLRSCR, NULL);
}

PUBLIC void disp_string(const char *str, byte color, byte x, byte y) {
    while (*str != 0)
        disp_char(*(str ++), color, x ++, y);
}

PUBLIC void disp_byte(byte num, byte color, byte x, byte y, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            disp_char('-', color, x ++, y);
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
        disp_char(S[top --] + '0', color, x ++, y);
    }
}

PUBLIC void disp_byte_hex(byte num, byte color, byte x, byte y, bool disp_0x) {
    if (disp_0x) {
        disp_char('0', color, x ++, y);
        disp_char('x', color, x ++, y);
    }

    char S[5];
    short top = 0;
    while (num) {
        S[++top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 2 - top;
    while (cnt --)
        disp_char('0', color, x ++, y);
    while (top) {
        disp_char(S[top --], color, x ++, y);
    }
}

PUBLIC void disp_short(word num, byte color, byte x, byte y, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            disp_char('-', color, x ++, y);
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
        disp_char(S[top --] + '0', color, x ++, y);
    }
}

PUBLIC void disp_short_hex(word num, byte color, byte x, byte y, bool disp_0x) {
    if (disp_0x) {
        disp_char('0', color, x ++, y);
        disp_char('x', color, x ++, y);
    }

    char S[7];
    short top = 0;
    while (num) {
        S[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 4 - top;
    while (cnt --)
        disp_char('0', color, x ++, y);
    while (top) {
        disp_char(S[top --], color, x ++, y);
    }
}

PUBLIC void disp_int(dword num, byte color, byte x, byte y, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            disp_char('-', color, x ++, y);
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
        disp_char(S[top --] + '0', color, x ++, y);
    }
}

PUBLIC void disp_int_hex(dword num, byte color, byte x, byte y, bool disp_0x) {
    if (disp_0x) {
        disp_char('0', color, x ++, y);
        disp_char('x', color, x ++, y);
    }

    char S[20];
    short top = 0;
    while (num) {
        S[++top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 8 - top;
    while (cnt --)
        disp_char('0', color, x ++, y);
    while (top) {
        disp_char(S[top --], color, x ++, y);
    }
}

//输出内容到vsprintf中
PUBLIC void vsprintf(char* buffer, const char* format, ...) {

}


PUBLIC void printf(const char* format, ...) {

}