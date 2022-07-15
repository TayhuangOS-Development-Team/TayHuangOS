/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong
 * 
 * command.h
 * 
 * 通用部分
 * 
 */



#pragma once

#include <tayhuang/video_info.h>

#define TEXT_WRITE_CHAR (0)
#define TEXT_WRITE_STRING (1)
#define CREATE_FRAMEBUFFER (2)
#define UPDATE_FRAMEBUFFER (3)

#define ARG_READ(args, type) *(type*)(((args) = (((void*)(args)) + sizeof(type))) - sizeof(type))
#define ARG_WRITE(args, type, value) *(type*)(((args) = (((void*)(args)) + sizeof(type))) - sizeof(type)) = value

void text_execute(int cmdid, void *args);

EXTERN PUBLIC video_info_struct video_info;