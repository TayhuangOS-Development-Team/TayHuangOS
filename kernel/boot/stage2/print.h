#pragma once

#include "header.h"

void dispchar(char ch, byte color, byte x, byte y);
void dispbyte(byte num, byte color, byte x, byte y, bool is_signed);
void dispbytehex(byte num, byte color, byte x, byte y, bool disp_0x);
void dispshort(short num, byte color, byte x, byte y, bool is_signed);
void dispshorthex(unsigned short num, byte color, byte x, byte y, bool disp_0x);
void dispstr(char* str, byte color, byte x, byte y);
void dispint(int num, byte color, byte x, byte y, bool is_signed);
void dispinthex(unsigned int num, byte color, byte x, byte y, bool disp_0x);
void clrscr(void);