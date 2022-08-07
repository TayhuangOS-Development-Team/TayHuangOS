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
 * __keyboard_driver_fn.c
 * 
 * keyboard_driver 函数功能
 * 
 */



#include <export/keyboard/__keyboard_driver_fn.h>

#include <tayhuang/services.h>

#include <syscall/rpc.h>
#include <memory/malloc.h>

#include <string.h>