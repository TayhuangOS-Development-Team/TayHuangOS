// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/file.h
 *
 * File functions are declared here
 */



#pragma once

#include "header.h"

typedef struct {
    int length;
    char name[32];
    char original_name[32];
    bool writable;
} file_info_t;

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

PUBLIC FILE* fopen(const char* name, const char* mode);
PUBLIC bool file_existed(const char* name);
PUBLIC void fclose(FILE* fp);
#define SEEK_SET (0)
#define SEEK_CUR (1)
#define SEEK_END (2)
PUBLIC void fseek(FILE *fp, int offset, int whence);
PUBLIC void fseekrd(FILE *fp, int offset, int whence);
PUBLIC void fseekwr(FILE *fp, int offset, int whence);
PUBLIC int feof(FILE *fp);