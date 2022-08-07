/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong
 * 
 * __video_driver_fn.c
 * 
 * video_driver 函数功能
 * 
 */



#include <export/video/__video_driver_fn.h>

#include <tayhuang/services.h>

#include <syscall/rpc.h>
#include <memory/malloc.h>

#include <string.h>

PUBLIC bool write_char(int column, int line, byte color, byte ch) {
    char buffer[TEXT_WRITE_CHAR_ARGS_SIZE];
    void *buf = buffer;

    ARG_WRITE(buf, int, column);
    ARG_WRITE(buf, int, line);
    ARG_WRITE(buf, byte, color);
    ARG_WRITE(buf, byte, ch);

    bool ret = remote_call(TEXT_WRITE_CHAR_RETURN_TYPE, VIDEO_DRIVER_SERVICE, TEXT_WRITE_CHAR_FN, MAKE_ARGS(buffer, TEXT_WRITE_CHAR_ARGS_SIZE));

    return ret;
}

PUBLIC bool write_string(int column, int line, byte color, const char *str) {
    int num = strlen(str);
    int size = num + TEXT_WRITE_STRING_ARGS_BASE_SIZE;

    char buffer = malloc(size);
    void *buf = buffer;

    ARG_WRITE(buf, int, column);
    ARG_WRITE(buf, int, line);
    ARG_WRITE(buf, byte, color);
    ARG_WRITE(buf, int, num);

    memcpy(buf, str, num);

    bool ret = remote_call(TEXT_WRITE_STRING_RETURN_TYPE, VIDEO_DRIVER_SERVICE, TEXT_WRITE_STRING_FN, MAKE_ARGS(buffer, size));
    
    return ret;
}

PUBLIC create_framebuffer_result create_framebuffer(int column, int line, int width, int height) {
    char buffer[CREATE_FRAMEBUFFER_ARGS_SIZE];
    void *buf = buffer;

    ARG_WRITE(buf, int, column);
    ARG_WRITE(buf, int, line);
    ARG_WRITE(buf, int, width);
    ARG_WRITE(buf, int, height);

    create_framebuffer_result framebuffer = remote_call(CREATE_FRAMEBUFFER_RETURN_TYPE, VIDEO_DRIVER_SERVICE, CREATE_FRAMEBUFFER_FN, MAKE_ARGS(buffer, CREATE_FRAMEBUFFER_ARGS_SIZE));

    return framebuffer;
}

PUBLIC bool swap_framebuffer(int id) {
    char buffer[SWAP_FRAMEBUFFER_ARGS_SIZE];
    void *buf = buffer;

    ARG_WRITE(buf, bool, id);

    bool ret = remote_call(SWAP_FRAMEBUFFER_RETURN_TYPE, VIDEO_DRIVER_SERVICE, SWAP_FRAMEBUFFER_FN, MAKE_ARGS(buffer, SWAP_FRAMEBUFFER_ARGS_SIZE));

    return ret;
}