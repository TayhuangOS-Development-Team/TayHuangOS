/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: Flysong
 * 
 * syscall.h
 * 
 * INT型系统调用
 * 
 */



#pragma once

int get_ticks(void);
int sleep(void);
int exit(void);
int wakeup(int pid);
int fatal(void);
int eggs(void);
int signal(int pid, int signal);