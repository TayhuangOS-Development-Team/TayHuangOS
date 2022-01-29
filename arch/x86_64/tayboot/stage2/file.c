// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/file.c
 *
 * File functions are implemented here
 */



#include "file.h"
#include "heap.h"
#include "buffer.h"
#include "drivers/disk/disk_driver.h"
#include "drivers/drivers.h"
#include <string.h>
#include "printf.h"

PRIVATE file_info_t* __get_file_info(const char* name) {
    file_info_t *info = malloc(sizeof(file_info_t));
    APACK(dk, load_file_info) args;
    args.name = name;
    args.info = info;
    if (! a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_LOAD_FILE_INFO, &args)) {
        free(info);
        return NULL;
    }
    return info;
}

PUBLIC bool file_existed(const char* name) {
    file_info_t *info = malloc(sizeof(file_info_t));
    APACK(dk, load_file_info) args;
    args.name = name;
    args.info = info;
    a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_LOAD_FILE_INFO, &args);
    if (info->length == -1) {    
        free(info);
        return false;
    }
    free(info);
    return true;
}

PRIVATE void __free_file_info(file_info_t *info) {
    free(info);
}

PUBLIC FILE* fopen(const char* name, const char* mode) {
#define FLAG_APPEND 1
#define FLAG_READONLY 2
#define FLAG_BINARY 4
    FILE *fp = malloc(sizeof(FILE));
    if (! file_existed(name)) {
        a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_CREATE_FILE, (char*)name);
    }
    fp->file_info = __get_file_info(name);
    if (! fp->file_info) {
        free(fp);
        return NULL;
    }
    fp->buffer_len = (fp->file_info->length == 0 ? 50 : fp->file_info->length * 2) + 10;
    fp->file_buffer = alloc_buffer(fp->buffer_len, false);
    APACK(dk, load_file) args;
    args.name = name;
    args.offset = fp->file_buffer;
    args.segment = get_buffer_seg();
    if (! a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_LOAD_FILE, &args)) {
        free_buffer(fp->file_buffer);
        __free_file_info(fp->file_info);
        free(fp);
        return NULL;
    }
    byte flags = 0;
    if (mode == NULL) {
        flags |= FLAG_BINARY;
        flags |= FLAG_READONLY;
    }
    else {
        do {
            if (*mode == 'w') {
                flags &= ~FLAG_READONLY;
            }
            else if (*mode == 'b') {
                flags |= FLAG_BINARY;
            }
            else if (*mode == 'a') {
                flags |= FLAG_APPEND;
            }
            else if (*mode == 'r') {
                flags |= FLAG_READONLY;
            }
        }
        while (*(++ mode) != '\0');
    }
    if (flags & FLAG_READONLY || (! fp->file_info->writable)) {
        fp->writable = false;
        fp->wrpos = -1;
        fp->replace = false;
    }
    else {
        fp->writable = true;
        fp->wrpos = 0;
        fp->replace = true;
    }
    if (flags & FLAG_BINARY) {
        fp->binary = true;
    }
    else {
        fp->binary = false;
    }
    if ((flags & FLAG_APPEND) && fp->file_info->writable) {
        fp->replace = false;
        fp->wrpos = fp->file_info->length;
    }
    else {
        fp->replace = true;
    }
    fp->rdpos = 0;
    return fp;
#undef FLAG_READONLY
#undef FLAG_BINARY
#undef FLAG_APPEND
}

PRIVATE void __save_file_info(const char* name, file_info_t* info) {
    APACK(dk, save_file_info) args;
    args.name = name;
    args.info = info;
    a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_SAVE_FILE_INFO, &args);
}

PRIVATE void __save_file(const char* name, addr_t buffer) {
    APACK(dk, save_file) args;
    args.name = name;
    args.offset = buffer;
    args.segment = get_buffer_seg();
    a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_SAVE_FILE, &args);
}

PUBLIC void fclose(FILE *fp) {
    if (fp->writable) {
        fp->file_info->length = fp->wrpos;
        __save_file_info(fp->file_info->name, fp->file_info);
        __save_file(fp->file_info->name, fp->file_buffer);
    }
    free_buffer(fp->file_buffer);
    __free_file_info(fp->file_info);
    free(fp);
}

PUBLIC void fseek(FILE *fp, int offset, int whence) {
    fseekrd(fp, offset, whence);
    fseekwr(fp, offset, whence);
}

PUBLIC void fseekrd(FILE *fp, int offset, int whence) {
    if (whence == SEEK_SET) {
        fp->rdpos = offset;
    }
    else if (whence == SEEK_CUR) {
        fp->rdpos += offset;
    }
    else if (whence == SEEK_SET) {
        fp->rdpos = fp->file_info->length - offset;
    }
}

PUBLIC void fseekwr(FILE *fp, int offset, int whence) {
    if (whence == SEEK_SET) {
        fp->wrpos = offset;
    }
    else if (whence == SEEK_CUR) {
        fp->wrpos += offset;
    }
    else if (whence == SEEK_SET) {
        fp->wrpos = fp->file_info->length - offset;
    }
}

PUBLIC int feof(FILE *fp) {
    if (fp->file_info->length >= fp->rdpos) return 0;
    return -1;
}