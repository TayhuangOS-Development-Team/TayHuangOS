#include "memory.h"

void _changeds(word segment);
void changeds(word segment) {
    _changeds(segment);
    ed_callasm();
}

void _restoreds(void);
void restoreds(void) {
    _restoreds();
    ed_callasm();
}

byte _getbyte(word segment, word offset);
byte getbyte(word segment, word offset) {
    static byte res;
    res = _getbyte(segment, offset);
    ed_callasm();
    return res;
}

word _getword(word segment, word offset);
word getword(word segment, word offset) {
    static word res;
    res = _getword(segment, offset);
    ed_callasm();
    return res;
}

dword _getdword(word segment, word offset);
dword getdword(word segment, word offset) {
    static dword res;
    res = _getdword(segment, offset);
    ed_callasm();
    return res;
}

void _setbyte(word segment, word offset, byte data);
void setbyte(word segment, word offset, byte data) {
    _setbyte(segment, offset, data);
    ed_callasm();
}

void _setword(word segment, word offset, word data);
void setword(word segment, word offset, word data) {
    _setword(segment, offset, data);
    ed_callasm();
}

void _setdword(word segment, word offset, dword data);
void setdword(word segment, word offset, dword data) {
    _setdword(segment, offset, data);
    ed_callasm();
}

void getbytearr(byte* arr, short len, word segment, word offset) {
    for (int i = 0 ; i < len ; i ++) {
        arr[i] = getbyte(segment, offset + i);
    }
}

void setbytearr(byte* arr, short len, word segment, word offset) {
    for (int i = 0 ; i < len ; i ++) {
        setbyte(segment, offset + i, arr[i]);
    }
}