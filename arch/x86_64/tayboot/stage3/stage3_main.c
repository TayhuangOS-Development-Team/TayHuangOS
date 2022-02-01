// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage3/stage3_main.c
 *
 * Stage3 main function here
 */



#include <boot_args.h>
#include <tay_defs.h>   
#include <string.h>
#include <ports.h>
#include "int_handlers.h"
#include "vedio.h"
#include "printf.h"
#include "init.h"
#include "memory.h"

void graphic_test(int width, int height) {
    color_rgb color;
    color.r = 0xFF;
    color.g = 0xFF;
    color.b = 0xFF;
    draw_rectangle(0, 0, width, height, &color);

    color.r = 0x40;
    color.g = 0x40;
    color.b = 0x40;
    //山
    //三竖
    draw_rectangle(60, 60, 100, 270, &color);
    draw_rectangle(170, 20, 210, 270, &color);
    draw_rectangle(280, 60, 320, 270, &color);
    //一横
    draw_rectangle(60, 270, 320, 310, &color);

    //本
    //两横
    draw_rectangle(440, 60, 780, 100, &color);
    draw_rectangle(520, 230, 700, 270, &color);
    //一竖
    draw_rectangle(590, 20, 630, 310, &color);
    //一撇
    for (int i = 0 ; i < 40 ; i ++) {
        draw_rectangle(550 - i * 3, 100 + i * 3, 590 - i * 3, 140 + i * 3, &color);
    }
    //一捺
    for (int i = 0 ; i <40 ; i ++) {
        draw_rectangle(630 + i * 3, 100 + i * 3, 670 + i * 3, 140 + i * 3, &color);
    }
}

void wait_for_disk_free(void) {
    while (inb(IDE1_STATUS) & 0x80);
}

void wait_for_drq(void) {
    while (inb(IDE1_STATUS) & 0x8);
}

bool volatile WAIT_FLAG = false;

void ide1_int_handler(int irq) {
    WAIT_FLAG = true;
}

#define MAKE_DEVICE(mode, driver, lba) (((mode) << 6) | (((driver) & 0x1) << 4) | ((lba) & 0xF) | 0xA0)

void read_sector(int lba, void* dst) {
    wait_for_disk_free();
    outb(IDE1_DEVICE, MAKE_DEVICE(1, 0, (lba & 0xF000000) >> 24));
    outb(IDE1_FEATURES, 0);
    outb(IDE1_DEVICE_CONTROL, 0);
    outb(IDE1_SECTOR_COUNTER, 1);
    outb(IDE1_LBA_LOW, (lba & 0x00000FF) >> 0);
    outb(IDE1_LBA_MID, (lba & 0x000FF00) >> 8);
    outb(IDE1_LBA_HIGH, (lba & 0x0FF0000) >> 16);
    outb(IDE1_COMMAND, 0x20);
    while (!WAIT_FLAG);
    WAIT_FLAG = false;
    for (int i = 0 ; i < 512 ; i += 2)
        *(word*)(dst + i) = inw(IDE1_DATA);
}

void read_sectors(int lba, int num, void* dst) {
    for (int i = 0 ; i < num ; i ++) {
        read_sector(lba + i, dst);
        dst += 512;
    }
}

#define BAILAN_IMAGE_ADDRESS (0x100000)
#define BAILAN_IMAGE_LBA_START (0)
#define BAILAN_IMAGE_WIDTH (256)
#define BAILAN_IMAGE_HEIGHT (192)

void init_screen(void) {
    color_rgb color;
    color.r = 0xFF;
    color.g = 0xFF;
    color.b = 0xFF;
    draw_rectangle(0, 0, 1024, 768, &color);
}

void read_photos(void) {
    // read_sectors(BAILAN_IMAGE_LBA_START, 124416, BAILAN_IMAGE_ADDRESS);
    read_sectors(BAILAN_IMAGE_LBA_START, 124416, BAILAN_IMAGE_ADDRESS);
}

PUBLIC int __times = 0;
int cur_frame = -1;

void draw_photo(dword idx) {
    char* address = BAILAN_IMAGE_ADDRESS + idx * BAILAN_IMAGE_WIDTH * BAILAN_IMAGE_HEIGHT * 3;
    for (dword i = 0 ; i < BAILAN_IMAGE_WIDTH ; i ++) {
        for (dword j = 0 ; j < BAILAN_IMAGE_HEIGHT ; j ++) {
            dword position0 = j * 2 * DPINFO.screen_width + i * 2;
            dword position1 = (j * 2 + 1) * DPINFO.screen_width + i * 2;
            dword position2 = j * 2 * DPINFO.screen_width + (i * 2 + 1);
            dword position3 = (j * 2 + 1) * DPINFO.screen_width + (i * 2 + 1);
            *(dword*)(DPINFO.framebuffer + position0 * 3) = *(dword*)address;
            *(dword*)(DPINFO.framebuffer + position1 * 3) = *(dword*)address;
            *(dword*)(DPINFO.framebuffer + position2 * 3) = *(dword*)address;
            *(dword*)(DPINFO.framebuffer + position3 * 3) = *(dword*)address;
            address += 3;
        }
    }
}

void timer(int irq) {
    __times ++;
    if (__times == 30) {
        __times = 0;
        if ((cur_frame != -1) && (cur_frame != 432))
            draw_photo(cur_frame ++);
    }
}

void start_bailan(void) {
    cur_frame = -1;
    init_screen();
    read_photos();
    cur_frame = 0;
}

void entry(struct boot_args* boot_args) {
    if (boot_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    outw(0x40, 65535);
    init_gdt();
    init_pic();
    init_idt();
    init_heap(boot_args->memory_size);
    init_vedio(boot_args->screen_width, boot_args->screen_height,
    boot_args->is_graphic_mode ? DPM_GRAPHIC : DPM_CHARACTER, boot_args->framebuffer);
    asmv ("sti");
    register_irq_handler(15, ide1_int_handler);
    register_irq_handler(0, timer);
    if (boot_args->is_graphic_mode) {
        //graphic_test(boot_args->screen_width, boot_args->screen_height);
        start_bailan();
    }
    else {
        print_info.scroll_line = 18;
        // printf ("STAGE3 Now!\n");
        // printf ("Boot args Magic:%#8X!\n", boot_args->magic);
        // printf ("Memory Size: %d (B), %d (KB), %d (MB)\n", boot_args->memory_size, boot_args->memory_size / 1024, boot_args->memory_size / 1024 / 1024);
    }
}