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
 * 文件的函数
 */



#include "file.h"
#include "heap.h"
#include "buffer.h"
#include "drivers/disk/disk_driver.h"
#include "drivers/drivers.h"
#include <string.h>
#include "printf.h"

PRIVATE file_info_t* __get_file_info(const char* name) { //获取文件信息
    file_info_t *info = malloc(sizeof(file_info_t));
    APACK(dk, load_file_info) args;
    args.name = name;
    args.info = info;
    //通过驱动实现
    if (! a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_LOAD_FILE_INFO, &args)) {
        free(info);
        return NULL;
    }
    return info;
}

PUBLIC bool file_existed(const char* name) {
    file_info_t *info = __get_file_info(name);
    if (info == NULL) //没找到
        return true;
    if (info->length == -1) { //长度为-1，还是没找到
        free(info);
        return false;
    }
    free(info);
    return true;
}

PRIVATE void __free_file_info(file_info_t *info) { //释放file info
    free(info);
}

PUBLIC FILE* fopen(const char* name, const char* mode) {
#define FLAG_APPEND 1
#define FLAG_READONLY 2
#define FLAG_BINARY 4 //标志位
    FILE *fp = malloc(sizeof(FILE));
    if (! file_existed(name)) { //不存在?
        //那就让它存在
        a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_CREATE_FILE, (char*)name);
    }
    //获取文件信息
    fp->file_info = __get_file_info(name);
    if (! fp->file_info) { //失败?
        //bye bye了您勒
        free(fp);
        return NULL;
    }
    //设置buffer长度
    fp->buffer_len = (fp->file_info->length == 0 ? 50 : fp->file_info->length * 2) + 10;
    //给buffer分配内存
    fp->file_buffer = alloc_buffer(fp->buffer_len, false);
    APACK(dk, load_file) args;
    args.name = name;
    args.offset = fp->file_buffer;
    args.segment = get_buffer_seg();
    if (! a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_LOAD_FILE, &args)) { //加载file到buffer中
        //失败?
        //bye bye了您勒
        free_buffer(fp->file_buffer);
        __free_file_info(fp->file_info);
        free(fp);
        return NULL;
    }
    byte flags = 0; //flag设为0
    if (mode == NULL) { //默认为READONLY
        flags |= FLAG_READONLY;
    }
    else {
        do {
            if (*mode == 'w') { //有w?
                //那就不是readonly
                flags &= ~FLAG_READONLY;
            }
            else if (*mode == 'b') { //有b?
                //那就是binary
                flags |= FLAG_BINARY;
            }
            else if (*mode == 'a') { //有a?
                //那就是append
                flags |= FLAG_APPEND;
            }
            else if (*mode == 'r') { //有r?
                //那就是readonly
                flags |= FLAG_READONLY;
            }
        }
        while (*(++ mode) != '\0');
    }
    if (flags & FLAG_READONLY || (! fp->file_info->writable)) { //如果是readonly或者文件本身是readonly
        //还想写?
        //醒醒吧您
        fp->writable = false;
        fp->wrpos = -1;
        fp->replace = false;
    }
    else { //如果不是
        //使劲写吧
        fp->writable = true;
        fp->wrpos = 0;
        fp->replace = true;
    }
    if (flags & FLAG_BINARY) { //如果2进制读入输出
        //安排上
        fp->binary = true;
    }
    else {
        //不要也罢
        fp->binary = false;
    }
    //其实fp->binary不必管它
    if ((flags & FLAG_APPEND) && ((flags & FLAG_READONLY) == 0)) { //如果可写且append
        fp->replace = false; //不必覆盖
        fp->wrpos = fp->file_info->length; //设在末尾
    }
    else {
        fp->replace = true; //覆盖吧
    }
    fp->rdpos = 0; //读入pos设为0
    return fp;
#undef FLAG_READONLY
#undef FLAG_BINARY
#undef FLAG_APPEND //可别让别人用这些东西
}

PRIVATE void __save_file_info(const char* name, file_info_t* info) { //保存文件信息
    //利用驱动
    APACK(dk, save_file_info) args;
    args.name = name;
    args.info = info;
    a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_SAVE_FILE_INFO, &args);
}

PRIVATE void __save_file(const char* name, addr_t buffer) { //保存文件内容
    //利用驱动
    APACK(dk, save_file) args;
    args.name = name;
    args.offset = buffer;
    args.segment = get_buffer_seg();
    a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_SAVE_FILE, &args);
}

PUBLIC void fclose(FILE *fp) {
    if (fp->writable) { //如果可写?
        //那就写呗!
        fp->file_info->length = fp->wrpos; //先设置长度
        __save_file_info(fp->file_info->name, fp->file_info); //保存信息
        __save_file(fp->file_info->name, fp->file_buffer); //保存内容
    }
    free_buffer(fp->file_buffer);
    __free_file_info(fp->file_info);
    free(fp); //释放缓存
}

PUBLIC void fseek(FILE *fp, int offset, int whence) {
    fseekrd(fp, offset, whence); //设置读入pos
    fseekwr(fp, offset, whence); //设置输出pos
}

PUBLIC void fseekrd(FILE *fp, int offset, int whence) {
    if (whence == SEEK_SET) { //相对于开头
        fp->rdpos = offset;
    }
    else if (whence == SEEK_CUR) { //相对于现在
        fp->rdpos += offset;
    }
    else if (whence == SEEK_SET) { //相对于结尾
        fp->rdpos = fp->file_info->length - offset;
    }
}

PUBLIC void fseekwr(FILE *fp, int offset, int whence) {
    if (whence == SEEK_SET) { //相对于开头
        fp->wrpos = offset;
    }
    else if (whence == SEEK_CUR) { //相对于现在
        fp->wrpos += offset;
    }
    else if (whence == SEEK_SET) { //相对于结尾
        fp->wrpos = fp->file_info->length - offset;
    }
}

PUBLIC int feof(FILE *fp) {
    if (fp->file_info->length >= fp->rdpos) return 0;
    return -1; //要是rdpos不比length下, 那肯定读完了
}