/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * stage2_main.c
 *
 * Stage2主函数
 *
 */



#include "drivers/drivers.h"
#include "drivers/devices.h"

#include "heap.h"
#include "console/console.h"
#include "drivers/disk/disk_driver.h"
#include "drivers/disk/filesystems/fs_fat16.h"
#include "pm/entry.h"
#include "printf.h"
#include "scanf.h"
#include "buffer.h"
#include "file.h"
#include <string.h>

#define MAX_TERMINATER_NUM (32) //Terminater最大数量
PRIVATE terminater_t TERMINATE_LIST[MAX_TERMINATER_NUM]; //Terminater表
PRIVATE byte temlst_sz = 0; //Terminater数量

PUBLIC bool register_terminater(terminater_t terminater) { //注册一个Terminater
    if (temlst_sz > MAX_TERMINATER_NUM) return false;
    TERMINATE_LIST[temlst_sz ++] = terminater;
    return true;
}

PUBLIC bool do_terminate(void) { //进行Terminater
    bool flag = true;
    for(byte i = 0 ; i < temlst_sz ; i ++) {
        flag &= TERMINATE_LIST[i]();
    }
    return flag;
}

void entry(void) {
    screen_info.height = 25;
    screen_info.width = 80;
    screen_info.current_line = 0;
    screen_info.scroll_line = 18; //初始化屏幕信息
    ll_init_buffer(); //初始化低级缓存

    init_devices(); //初始化设备
    init_drivers(); //初始化驱动
    clrscr(); //清屏

    init_buffer(); //初始化缓存

    init_heap(); //初始化堆

    a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_INIT, NULL); //初始化硬盘驱动

    init_key_buffer(); //初始化击键缓存

#ifdef GOTO_OS_AUTO
    go_to_protect_mode(); //进入保护模式
#else
    enter_console(); //进入realmode console
#endif

    do_terminate(); //清理
}