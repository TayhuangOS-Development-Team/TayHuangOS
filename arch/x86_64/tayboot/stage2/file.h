/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/file.h
 *
 * File functions are declared here
 *
 */



#pragma once

#include "header.h"

//文件信息
typedef struct {
    int length;
    char name[32];
    char original_name[32];
    bool writable;
} file_info_t;

//FILE的结构
//其实应该隐藏的
//但是我懒
typedef struct {
    addr_t file_buffer;
    file_info_t* file_info;
    int rdpos;
    int wrpos;
    int buffer_len;
    bool writable;
    bool replace;
    bool binary;
}FILE;

//打开文件
PUBLIC FILE* fopen(const char *name, const char *mode);
//文件是否存在
PUBLIC bool file_existed(const char *name);
//关闭文件
PUBLIC void fclose(FILE* fp);
#define SEEK_SET (0) //相对于开头
#define SEEK_CUR (1) //相对于目前
#define SEEK_END (2) //相对于结尾
//设置文件位置
PUBLIC void fseek(FILE *fp, int offset, int whence);
//设置文件读位置
PUBLIC void fseekrd(FILE *fp, int offset, int whence);
//设置文件写位置
PUBLIC void fseekwr(FILE *fp, int offset, int whence);
//测试是否到文件尾
PUBLIC int feof(FILE *fp);