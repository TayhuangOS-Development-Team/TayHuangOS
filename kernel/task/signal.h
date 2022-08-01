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
 * signal.h
 * 
 * 信号量
 * 
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC id_t create_signal(qword max_signals);
PUBLIC void up_signal(id_t signal);
PUBLIC void down_signal(id_t signal);