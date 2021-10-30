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
    char S[5] = {};
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

void dispbytehex(byte num, byte color, byte x, byte y, bool disp_0x) {
    if (disp_0x) {
        dispchar('0', color, x ++, y);
        dispchar('x', color, x ++, y);
    }

    if (((num >> 4) & 0xF) >= 10) {
        dispchar(((num >> 4) & 0xF) - 10 + 'A', color, x ++, y);
    }
    else {
        dispchar(((num >> 4) & 0xF) + '0', color, x ++, y);
    }

    if ((num & 0xF) >= 10) {
        dispchar((num & 0xF) - 10 + 'A', color, x, y);
    }
    else {
        dispchar((num & 0xF) + '0', color, x, y);
    }
}


void dispshort(short num, byte color, byte x, byte y, bool is_signed) {
    if (is_signed) {
        if (num < 0) {
            dispchar('-', color, x ++, y);
            num = - num;
        }
    }
    char S[7] = {};
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

void dispshorthex(short num, byte color, byte x, byte y, bool disp_0x) {
    if (disp_0x) {
        dispchar('0', color, x ++, y);
        dispchar('x', color, x ++, y);
    }

    if (((num >> 12) & 0xF) >= 10) {
        dispchar(((num >> 12) & 0xF) - 10 + 'A', color, x ++, y);
    }
    else {
        dispchar(((num >> 12) & 0xF) + '0', color, x ++, y);
    }

    if (((num >> 8) & 0xF) >= 10) {
        dispchar(((num >> 8) & 0xF) - 10 + 'A', color, x ++, y);
    }
    else {
        dispchar(((num >> 8) & 0xF) + '0', color, x ++, y);
    }

    if (((num >> 4) & 0xF) >= 10) {
        dispchar(((num >> 4) & 0xF) - 10 + 'A', color, x ++, y);
    }
    else {
        dispchar(((num >> 4) & 0xF) + '0', color, x ++, y);
    }

    if ((num & 0xF) >= 10) {
        dispchar((num & 0xF) - 10 + 'A', color, x, y);
    }
    else {
        dispchar((num & 0xF) + '0', color, x, y);
    }
}

void _clrscr(void);
void clrscr(void) {
    _clrscr();
    ed_callasm();
}