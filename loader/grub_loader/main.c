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
 * main.c
 *
 * GRUB Loader主程序
 *
 */



#include "multiboot2.h"
#include "printf.h"
#include "init.h"

#ifdef VBE_ENABLE
    #define HEADER_LENGTH (24)
#else
    #define HEADER_LENGTH (24)
#endif

#define MBSC __attribute__((section(".multiboot")))

struct multiboot_header mbheader MBSC = {
    .magic    = MULTIBOOT2_HEADER_MAGIC,
    .architecture = MULTIBOOT_ARCHITECTURE_I386,
    .header_length = HEADER_LENGTH,
    .checksum = -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + HEADER_LENGTH),
};

#ifdef VBE_ENABLE
struct multiboot_tag_framebuffer mbframebuffer MBSC = {
};
#else
#endif


struct multiboot_header_tag mbend MBSC = {
    .type = MULTIBOOT_HEADER_TAG_END,
    .flags = 0,
    .size = 8
};


void entry(void) {
    write_str ("Hello, OS World!", 0x0F, 0, 0);
    init_gdt();
    write_str ("Hello, OS World!", 0x0F, 0, 1);
    while (1);
}