#pragma once

#include "header.h"

void changeds(word segment);
void restoreds(void);
byte getbyte(word segment, word offset);
word getword(word segment, word offset);
dword getdword(word segment, word offset);
void setbyte(word segment, word offset, byte data);
void setword(word segment, word offset, word data);
void setdword(word segment, word offset, dword data);
void getbytearr(byte* arr, short len, word segment, word offset);
void setbytearr(byte* arr, short len, word segment, word offset);