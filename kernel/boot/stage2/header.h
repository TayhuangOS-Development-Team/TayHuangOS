#pragma once

#ifdef _WIN32
    #define ed_callasm()
#else
    #define ed_callasm() asm("addw $2, %sp")
#endif

#define bool unsigned char
#define byte unsigned char
#define true 1
#define false 0