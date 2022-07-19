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
 * services.h
 * 
 * 内核模块服务号
 * 
 */



#pragma once

#define API_PID(x) (0x10000 + (x))

//init()
#define INIT_SERVICE (1)
//setup.mod 辅助内核初始化
#define SETUP_SERVICE API_PID(0)
//void sys_task() 系统调用模块
#define SYSTASK_SERVICE API_PID(1)
//保留
#define RESERVED0_SERVICE API_PID(2)
//video.mod 视频驱动
#define VIDEO_DRIVER_SERVICE API_PID(3)
//tty.mod tty虚拟设备驱动
#define TTY_DRIVER_SERVICE API_PID(4)
//keyboard.mod 键盘驱动
#define KEYBOARD_DRIVER_SERVICE API_PID(5)
//disk.mod 硬盘驱动
#define DISK_DRIVER_SERVICE API_PID(6)
//vfs.mod 文件系统虚拟设备驱动
#define VFS_DRIVER_SERVICE API_PID(7)

//空进程
#define NULL_TASK (0)
//任意进程
#define ANY_TASK (-1)
//假进程
#define DUMMY_TASK (-2)