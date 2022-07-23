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
 * framebuffers.h
 * 
 * framebuffer表
 * 
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC void *get_framebuffer(int pid);
PUBLIC void set_framebuffer(int pid, void *framebuffer);
PUBLIC bool has_framebuffer(int pid);
PUBLIC void remove_framebuffer(int pid);