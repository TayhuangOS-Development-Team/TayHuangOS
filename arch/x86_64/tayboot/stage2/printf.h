/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong Beanflame
 *
 * arch/x86_64/tayboot/stage2/printf.h
 *
 * Display functions are declared here
 *
 */



#pragma once

#include "header.h"
#include "file.h"

//屏幕信息
struct screen_info_t{
    int width;
    int height;
    int current_line;
    int scroll_line;
};

extern struct screen_info_t screen_info;

//打印字符到屏幕中
PUBLIC void disp_char(char ch, byte color, byte x, byte y);
//改变打印位置(x, y)
PUBLIC void change_pos(word x, word y);
//获取打印位置(x, y)
PUBLIC word get_pos_x(void);
PUBLIC word get_pos_y(void);
PUBLIC void putchar(char ch); //打印字符到屏幕中
PUBLIC void puts(const char *str); //打印字符串到屏幕中

PUBLIC void clrscr(void); //清屏
PUBLIC byte get_print_color(void); //获取输出的颜色
PUBLIC void set_print_color(byte color); //设置输出的颜色

PUBLIC int vsprintf(char *buffer, const char *format, ...); //打印内容到buffer中
PUBLIC int printf(const char *format, ...); //打印内容到屏幕中
PUBLIC int fprintf(FILE *file, const char *format, ...); //打印内容到文件中

PUBLIC void fputs(FILE *file, const char *str); //打印字符串到文件中
PUBLIC void fputc(FILE *file, char ch); //打印字符到文件中
PUBLIC void scroll_screen(int offset); //滚动屏幕