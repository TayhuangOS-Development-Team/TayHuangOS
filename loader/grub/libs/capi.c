/**
 * @file capi.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief CAPI
 * @version alpha-1.0.0
 * @date 2022-12-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <libs/capi.h>
#include <tay/types.h>

static byte __HEAP__[HEAP_SIZE];

static byte *__heap_ptr__ = __HEAP__;

void *malloc(size_t size) {
    void *ret = __heap_ptr__;
    __heap_ptr__ += size;
    return ret;
}

void free(void *ptr) {
    // 空实现
}

static word print_pos_x = 0;
static word print_pos_y = 0;
static const word char_per_line = 80;
static word *VIDEO_MEMORY = 0xB8000;
static const byte print_color = 0x0F;

static void putrawchar(char ch) {
    VIDEO_MEMORY[print_pos_x + print_pos_y * 80] = (((print_color & 0xFF) << 8) + (ch & 0xFF));
}

void putchar(char ch) {
    switch (ch) {
        case '\r':
        case '\n': {
            print_pos_x = 0;
            print_pos_y ++;
            break;
        }
        case '\t': {
            print_pos_x += 4;
            break;
        }
        case '\v': {
            print_pos_y ++;
            break;
        }
        case '\f': {
            print_pos_x = 0;
            print_pos_y = 0;
            // clrscr();
            break;
        }
        case '\b': {
            print_pos_x --;
            putrawchar(' ');
            break;
        }
        default: {
            putrawchar(ch);
            print_pos_x ++;
        }
    }
    
    if (print_pos_x >= char_per_line) { //自动换行
        print_pos_x -= char_per_line;
        print_pos_y ++;
    }
}

void puts(const char *str) {
    while (*str != '\0') {
        putchar(*str);
        str ++;
    }
}