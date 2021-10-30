#include "print.h"

void _dispchar(char ch, byte color, short pos);
void dispchar(char ch, byte color, byte x, byte y) {
    _dispchar(ch, color, y * 80 + x);
    ed_callasm();
}

void dispbyte(byte num, byte color, byte x, byte y, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            dispchar('-', color, x ++, y);
            num = - num;
        }
    }
    char S[5];
    short top = 0;
	while(num) {
        S[++ top] = num % 10;
        num /= 10;
    }
	if (! top) S[++ top] = 0;
	while (top) {
        dispchar (S[top --] + '0', color, x ++, y);
    }
}

void dispbytehex(byte num, byte color, byte x, byte y, bool disp_0x) {
    if (disp_0x) {
        dispchar('0', color, x ++, y);
        dispchar('x', color, x ++, y);
    }

    char S[5];
    short top = 0;
    while(num) {
        S[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 2 - top;
    while (cnt --) dispchar('0', color, x ++, y);
	while (top) {
        dispchar (S[top --], color, x ++, y);
    }
}

void dispshort(short num, byte color, byte x, byte y, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            dispchar('-', color, x ++, y);
            num = - num;
        }
    }
    char S[7];
    short top = 0;
	while(num) {
        S[++ top] = num % 10;
        num /= 10;
    }
	if (! top) S[++ top]=0;
	while (top) {
        dispchar (S[top --] + '0', color, x ++, y);
    }
}

void dispshorthex(unsigned short num, byte color, byte x, byte y, bool disp_0x) {
    if (disp_0x) {
        dispchar('0', color, x ++, y);
        dispchar('x', color, x ++, y);
    }

    char S[7];
    short top = 0;
    while(num) {
        S[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 4 - top;
    while (cnt --) dispchar('0', color, x ++, y);
	while (top) {
        dispchar (S[top --], color, x ++, y);
    }
}

void dispint(int num, byte color, byte x, byte y, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            dispchar('-', color, x ++, y);
            num = - num;
        }
    }
    char S[14];
    short top = 0;
	while(num) {
        S[++ top] = num % 10;
        num /= 10;
    }
	if (! top) S[++ top]=0;
	while (top) {
        dispchar (S[top --] + '0', color, x ++, y);
    }
}

void dispinthex(unsigned int num, byte color, byte x, byte y, bool disp_0x) {
    if (disp_0x) {
        dispchar('0', color, x ++, y);
        dispchar('x', color, x ++, y);
    }

    char S[20];
    short top = 0;
    while(num) {
        S[++ top] = (num & 0xF) >= 10 ? (num & 0xF) - 10 + 'A' : (num & 0xF) + '0';
        num >>= 4;
    }
    char cnt = 8 - top;
    while (cnt --) dispchar('0', color, x ++, y);
	while (top) {
        dispchar (S[top --], color, x ++, y);
    }
}

void dispstr(char* str, byte color, byte x, byte y) {
    while (*str != 0)
        dispchar(*(str ++), color, x ++, y);
}

void _clrscr(void);
void clrscr(void) {
    _clrscr();
    ed_callasm();
}