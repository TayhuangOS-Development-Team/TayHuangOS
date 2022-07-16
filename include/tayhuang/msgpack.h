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
 * msgpack.h
 * 
 * 消息包结构
 * 
 */



#pragma once

#include <tayhuang/defs.h>

typedef struct __msgpack_struct {
    qword length : 44;
    dword source : 20;
} msgpack_struct;