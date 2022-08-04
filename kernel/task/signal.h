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

//创建信号量
PUBLIC id_t __create_signal(int max_signals, int value, bool soft);
//up/down原语
PUBLIC void __up_signal(id_t id);
PUBLIC void __down_signal(id_t id);
//获得信号量
PUBLIC int __get_signals(id_t id);
//是否为soft信号量
PUBLIC bool __is_soft_signal(id_t id);