/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: Flysong
 * 
 * printk.c
 * 
 * 内核printk
 * 
 */



#include "printk.h"
#include "video.h"
#include "../kheap.h"
#include <tool/tostring.h>
#include <stdarg.h>
#include <tayhuang/io.h>

#define OUTPUT_BUFFER_SIZE (1024)

PRIVATE char output_buffer[OUTPUT_BUFFER_SIZE];
PRIVATE int current_pos = 0;
PRIVATE int start_pos = 0;
PRIVATE int print_color = 0;
PRIVATE int scroll_line = 0;

PRIVATE void set_cursor_pos(int pos) {
    outb(0x3D4, 0x0F);
	outb(0x3D5, (byte) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (byte) ((pos >> 8) & 0xFF));
}

PUBLIC int get_print_color(void) {
    return print_color;
}

PUBLIC void set_print_color(int color) {
    print_color = color;
}

PUBLIC void change_pos(int x, int y) {
    current_pos = y * VIDEO_INFO.width + x;
    set_cursor_pos(current_pos);
}

PUBLIC int get_pos_x(void) {
    return current_pos % VIDEO_INFO.width;
}

PUBLIC int get_pos_y(void) {
    return current_pos / VIDEO_INFO.width;
}

PUBLIC int get_scroll_line(void) {
    return scroll_line;
}

PUBLIC void set_scroll_line(int line) {
    scroll_line = line;
}

PUBLIC void clrscr(void) {
    memset(VIDEO_INFO.framebuffer, 0, VIDEO_INFO.width * VIDEO_INFO.height * 2);
    start_pos = 0;
    current_pos = 0;
}

PRIVATE void flush_to_screen(void) {
    memset(VIDEO_INFO.framebuffer, 0, VIDEO_INFO.width * VIDEO_INFO.height * 2);
    for (int i = start_pos ; i < current_pos ; i ++) {
        int position = i - start_pos;
        draw_character(position % VIDEO_INFO.width, position / VIDEO_INFO.width, output_buffer[i], print_color);
    }
}

PUBLIC void scroll_screen(int lines) {
    start_pos += lines * VIDEO_INFO.width;
    flush_to_screen();
}

PUBLIC void putchar(char ch) {
    if (ch == '\r' || ch == '\n') { //制表符
        int line = current_pos / VIDEO_INFO.width;
        current_pos = (line + 1) * VIDEO_INFO.width;
        flush_to_screen();
        set_cursor_pos(current_pos - start_pos);
    }
    else if (ch == '\t') {
        current_pos += 4;
        flush_to_screen();
        set_cursor_pos(current_pos - start_pos);
    }
    else if (ch == '\v') {
        current_pos += VIDEO_INFO.width;
        flush_to_screen();
        set_cursor_pos(current_pos - start_pos);
    }
    else if (ch == '\b') {
        current_pos --;
        set_cursor_pos(current_pos - start_pos);
    }
    else if(ch == '\f') {
        clrscr();
        flush_to_screen();
        current_pos = 0;
        start_pos = 0;
    }
    else { //普通字符
        output_buffer[current_pos ++] = ch;
    }
    if (((current_pos - start_pos) / VIDEO_INFO.width) - scroll_line > 0) { //滚动屏幕
        scroll_screen(((current_pos - start_pos) / VIDEO_INFO.width) - scroll_line);
        set_cursor_pos(current_pos - start_pos);
    }
}

PUBLIC void puts(const char *str) {
    while (*str)
        putchar (*(str ++));
}

PRIVATE int _vsprintk(char *buffer, const char *format, va_list args) {

}

PUBLIC int printk(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[128];
    
    int ret = _vsprintk(buffer, format, args);

    puts(buffer);

    va_end(args);
    return ret;
}

PUBLIC int sprintk(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    int ret = _vsprintk(buffer, format, args);

    va_end(args);
    return ret;
}