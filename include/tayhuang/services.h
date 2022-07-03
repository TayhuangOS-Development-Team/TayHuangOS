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
 * services.h
 * 
 * 内核模块服务号
 * 
 */



#pragma once

#define API_PID(x) (0x10000 + (x))

//setup.mod 辅助内核初始化
#define SETUP_SERVICE API_PID(0)
//void mm() 内存管理模块
#define MM_SERVICE API_PID(1)
//void taskman() 任务管理模块
#define TASKMAN_SERVICE API_PID(2)
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