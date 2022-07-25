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
 * show_icon.c
 * 
 * 显示图标
 * 
 */



#include <show_icon.h>

#include <tayhuang/ports.h>
#include <tayhuang/io.h>

#include <disk.h>
#include <fs/common.h>

#include <int_handlers.h>
#include <lheap.h>

#define PIT_FREQUENCY (1193181.6666f)

PUBLIC bool init_pit(float frequency) { //初始化PIT
    if (frequency > PIT_FREQUENCY) { //频率过高
        return false;
    }

    int count = (int)(PIT_FREQUENCY / frequency);
    if ((PIT_FREQUENCY - count * frequency) > (frequency / 2)) {
        count ++;
    }

    if (count >= 65535) return false; //频率过低

    outb(PIT_CHANNEL0, (byte)count); //设置频率
    outb(PIT_CHANNEL0, (byte)(count >> 8)); //设置频率

    return true;
}

#ifdef VBE_ENABLE

PRIVATE VOLATILE int progress = -1;
PRIVATE VOLATILE int cnt = 0;

#define CLOCKS_PER_SECOND (100)
#define ANIMATION_TIME (4)

PUBLIC void clock_irq_handler(int irq) {
    if (progress != -1 && progress < 100) {
        cnt ++;
        if (cnt >= (ANIMATION_TIME * 100 / CLOCKS_PER_SECOND)) {
            cnt = 0;
            progress ++;
        }
    }
}

#define ICON_SIZE (1028 * 1024)

PRIVATE void clear_screen(void *framebuffer, int width, int height) {
    for (int i = 0 ; i < height ; i ++) {
        for (int j = 0 ; j < width ; j ++) {
            *(byte *)(framebuffer + (i * width + j) * 3 + 0) = 0xFF;
            *(byte *)(framebuffer + (i * width + j) * 3 + 1) = 0xFF;
            *(byte *)(framebuffer + (i * width + j) * 3 + 2) = 0xFF;
        }
    }
}

PRIVATE void display_icon(void *icon, void *framebuffer, int width, int height) {
    asmv ("cli");

    int ic_width = *(int *)icon;
    int ic_height = *(int *)(icon + 4);
    icon += 8;

    //居中
    int offset_x = width / 2 - ic_width / 2;
    int offset_y = height / 2 - ic_height / 2;

    for (int i = 0 ; i < ic_height ; i ++) {
        for (int j = 0 ; j < ic_width ; j ++) {
            dword __r =  *(byte *)(icon + (i * ic_width + j) * 4 + 0);
            dword __g =  *(byte *)(icon + (i * ic_width + j) * 4 + 1);
            dword __b =  *(byte *)(icon + (i * ic_width + j) * 4 + 2);
            dword gray = (__r * 30 + __g * 59 + __b * 11 + 50) / 100;

            dword r = __r * progress / 100 + gray * (100 - progress) / 100;
            dword g = __g * progress / 100 + gray * (100 - progress) / 100;
            dword b = __b * progress / 100 + gray * (100 - progress) / 100;

            *(byte *)(framebuffer + ((i + offset_y) * width + j + offset_x) * 3 + 0) = b;
            *(byte *)(framebuffer + ((i + offset_y) * width + j + offset_x) * 3 + 1) = g;
            *(byte *)(framebuffer + ((i + offset_y) * width + j + offset_x) * 3 + 2) = r;
        }
    }

    asmv ("sti");
}

PUBLIC void show_icon(void *framebuffer, int width, int height) {
    progress = 0;

    init_pit(CLOCKS_PER_SECOND);

    register_irq_handler(0, clock_irq_handler);
    enable_irq(0);

    clear_screen(framebuffer, width, height);

    fs_context ctx = load_fs(DISK_SEL_IDE0, 0);

    void *icon = lmalloc(ICON_SIZE);

    load_file(ctx, "tayicon.raw", icon);

    terminate_fs(ctx);

    VOLATILE int last_progress = -1;
    while (last_progress < 100) {
        if (last_progress < progress) {
            display_icon(icon, framebuffer, width, height);
        }
        last_progress = progress;
    }

    lfree (icon);
}

#endif