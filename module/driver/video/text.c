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
 * command.c
 * 
 * 命令
 * 
 */



#include <global.h>
#include <framebuffers.h>
#include <export/__video_driver_fn.h>

#include <debug/logging.h>

#include <syscall/syscall.h>
#include <syscall/ipc.h>
#include <syscall/rpc.h>

#include <memory/malloc.h>
#include <memory/sharemem.h>

#include <tayhuang/paging.h>

static inline void __write_char(int column, int line, byte color, byte ch) {
    *(byte *)(video_info.framebuffer + ((line * video_info.width) + column) * 2 + 0) = ch;
    *(byte *)(video_info.framebuffer + ((line * video_info.width) + column) * 2 + 1) = color;
}

PRIVATE rpc_args_struct wrapper_write_char(int caller, rpc_func func_no, rpc_args_struct args) {
    int column = ARG_READ(args.data, int);
    int line = ARG_READ(args.data, int);
    byte color = ARG_READ(args.data, byte);
    byte ch = ARG_READ(args.data, byte);

    color &= 0xFF;
    ch &= 0xFF;

    __write_char(column, line, color, ch);

    bool *result = malloc(sizeof(TEXT_WRITE_CHAR_RETURN_TYPE));
    *result = true;

    return (rpc_args_struct){.data = result, .size = sizeof(TEXT_WRITE_CHAR_RETURN_TYPE)};
}

PRIVATE rpc_args_struct wrapper_write_string(int caller, rpc_func func_no, rpc_args_struct args) {
    bool *result = malloc(sizeof(TEXT_WRITE_STRING_RETURN_TYPE));

    if (args.size < TEXT_WRITE_STRING_ARGS_BASE_SIZE) {
        *result = false;
        return (rpc_args_struct){.data = result, .size = sizeof(TEXT_WRITE_STRING_RETURN_TYPE)};
    }

    int column = ARG_READ(args.data, int);
    int line = ARG_READ(args.data, int);
    byte color = ARG_READ(args.data, byte);
    int num = ARG_READ(args.data, int);

    if (args.size != (TEXT_WRITE_STRING_ARGS_BASE_SIZE + num)) {
        *result = false;
        return (rpc_args_struct){.data = result, .size = sizeof(TEXT_WRITE_STRING_RETURN_TYPE)};
    }


    color &= 0xFF;

    for (int i = 0 ; i < num ; i ++) {
        byte ch = ARG_READ(args.data, byte);
        ch &= 0xFF;
        __write_char(column + i, line, color, ch);
    }

    *result = true;

    return (rpc_args_struct){.data = result, .size = sizeof(TEXT_WRITE_STRING_RETURN_TYPE)};
}

PRIVATE void *__create_framebuffer(int caller, int column, int line, int width, int height) {
    if (width < 0 || width >= video_info.width) {
        return NULL;
    }

    if (height < 0 || height >= video_info.height) {
        return NULL;
    }

    int bpp = video_info.is_graphic_mode ? 2 : 4;
    int size = width * height * bpp;
    int pages = (size + MEMUNIT_SZ - 1) / MEMUNIT_SZ;
    
    void *__framebuffer = create_share_memory(pages);
    void *framebuffer = share_memory(__framebuffer, pages, caller);

    set_framebuffer(caller, __framebuffer, column, line, width, height);

    return framebuffer;
}

PRIVATE rpc_args_struct wrapper_create_framebuffer(int caller, rpc_func func_no, rpc_args_struct args) {
    int column = ARG_READ(args.data, int);
    int line = ARG_READ(args.data, int);
    int width = ARG_READ(args.data, int);
    int height = ARG_READ(args.data, int);

    void **result = (void **)malloc(sizeof(void *));
    *result = __create_framebuffer(caller, column, line, width, height);

    return (rpc_args_struct){.data = result, .size = sizeof(CREATE_FRAMEBUFFER_RETURN_TYPE)};
}

PRIVATE void __swap_framebuffer(int caller, bool use_default_color) {
    frame_t *frame = get_framebuffer(caller);
    void *framebuffer = frame->framebuffer;

    if (! use_default_color) {
        for (int i = 0 ; i < frame->height ; i ++) {
            for (int j = 0 ; j < frame->width ; j ++) {
                int pos = (i * frame->width + j) * 2;
                byte ch = *(byte *)(framebuffer + pos);
                byte color = *(byte *)(framebuffer + pos + 1);

                write_char(frame->column + j, frame->height + i, color, ch);
            }
        }
    }
    else {
        for (int i = 0 ; i < frame->height ; i ++) {
            for (int j = 0 ; j < frame->width ; j ++) {
                int pos = (i * frame->width + j) * 2;
                byte ch = *(byte *)(framebuffer + pos);

                write_char(frame->column + j, frame->height + i, 0x0F, ch);
            }
        }
    }
}

PRIVATE rpc_args_struct wrapper_swap_framebuffer(int caller, rpc_func func_no, rpc_args_struct args) {
    bool use_default_color = ARG_READ(args.data, bool);

    __swap_framebuffer(caller, use_default_color);

    bool *result = malloc(sizeof(SWAP_FRAMEBUFFER_RETURN_TYPE));
    *result = true;

    return (rpc_args_struct){.data = result, .size = sizeof(SWAP_FRAMEBUFFER_RETURN_TYPE)};
}

PUBLIC void text_register_rpc_functions(void) {
    rpc_register(TEXT_WRITE_CHAR_FN, wrapper_write_char, sizeof(TEXT_WRITE_CHAR_RETURN_TYPE), TEXT_WRITE_CHAR_ARGS_SIZE);
    rpc_register(TEXT_WRITE_STRING_FN, wrapper_write_string, sizeof(TEXT_WRITE_STRING_RETURN_TYPE), TEXT_WRITE_STRING_ARGS_SIZE);
    rpc_register(CREATE_FRAMEBUFFER_FN, wrapper_create_framebuffer, sizeof(CREATE_FRAMEBUFFER_RETURN_TYPE), CREATE_FRAMEBUFFER_ARGS_SIZE);
    rpc_register(SWAP_FRAMEBUFFER_FN, wrapper_swap_framebuffer, sizeof(SWAP_FRAMEBUFFER_RETURN_TYPE), SWAP_FRAMEBUFFER_ARGS_SIZE);
}