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
 * main.c
 *
 * GRUB Loader主程序
 *
 */



#include <libs/multiboot2.h>
#include <tayhuang/attributes.h>
#include <tayhuang/types.h>

//Tayhuang OS GRUB Loader Multiboot2 header struct
struct tayhuang_header {
    struct multiboot_header header;
#ifdef VBE_ENABLE
    struct multiboot_header_tag_framebuffer framebuffer;
    multiboot_uint32_t reserved0;
#endif
    struct multiboot_header_tag end;
} __attribute__((packed));

//将这个头放在.multiboot段下
struct tayhuang_header TAYHUANG_HEADER __attribute__((section(".multiboot"))) = {
    .header = {
        .magic    = MULTIBOOT2_HEADER_MAGIC,
        .architecture = MULTIBOOT_ARCHITECTURE_I386,
        .header_length = sizeof (struct tayhuang_header),
        .checksum = -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + sizeof(struct tayhuang_header)),
    },
#ifdef VBE_ENABLE
    #define FRAMEBUFFER_WIDTH 1024
    #define FRAMEBUFFER_HEIGHT 768
    #define FRAMEBUFFER_BPP 24
    .framebuffer = {
        .type = MULTIBOOT_HEADER_TAG_FRAMEBUFFER,
        .flags = MULTIBOOT_HEADER_TAG_OPTIONAL,
        .size = sizeof (struct multiboot_header_tag_framebuffer),
        .width = FRAMEBUFFER_WIDTH,
        .height = FRAMEBUFFER_HEIGHT,
        .depth = FRAMEBUFFER_BPP
    },
#endif
    .end = {
        .type = MULTIBOOT_HEADER_TAG_END,
        .flags = 0,
        .size = sizeof (struct multiboot_header_tag)
    }
};

//loader入口点
PUBLIC void entry(void) {
    *(word*)(0xB8000) = 0x0C31;
    *(word*)(0xB8002) = 0x0C31;
    *(word*)(0xB8004) = 0x0C34;
    *(word*)(0xB8006) = 0x0C35;
    *(word*)(0xB8008) = 0x0C31;
    *(word*)(0xB800A) = 0x0C34;
    while (true);
}