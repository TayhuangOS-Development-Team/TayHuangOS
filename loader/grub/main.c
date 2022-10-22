/**
 * @file main.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief GRUB 2 Loader 入口
 * @version alpha-1.0.0
 * @date 2022-10-22
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <libs/multiboot2.h>
#include <tayhuang/attributes.h>
#include <tayhuang/types.h>

// GRUB 2 API
IMPL("GRUB2") 
// 程序头结构
struct tayhuang_header {
    // GRUB 2头
    struct multiboot_header header;
    // 启用VBE
#ifdef VBE_ENABLE
    // 屏幕信息
    struct multiboot_header_tag_framebuffer framebuffer;
    // 对齐
    multiboot_uint32_t reserved0;
#endif
    // GRUB 2尾
    struct multiboot_header_tag end;
} __attribute__((packed));

// Grub2 API
IMPL("GRUB2") 
// 将这个头放在.multiboot段下
struct tayhuang_header TAYHUANG_HEADER 
//将这个头放在.multiboot段下
__attribute__((section(".multiboot")))
// 程序头内容 
= {
    //GRUB 2头
    .header = {
        .magic    = MULTIBOOT2_HEADER_MAGIC,
        .architecture = MULTIBOOT_ARCHITECTURE_I386,
        .header_length = sizeof (struct tayhuang_header),
        .checksum = -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + sizeof(struct tayhuang_header)),
    },
    //启用VBE
#ifdef VBE_ENABLE
    //屏幕基本信息
    #define FRAMEBUFFER_WIDTH 1024
    #define FRAMEBUFFER_HEIGHT 768
    #define FRAMEBUFFER_BPP 24
    //屏幕信息
    .framebuffer = {
        .type = MULTIBOOT_HEADER_TAG_FRAMEBUFFER,
        .flags = MULTIBOOT_HEADER_TAG_OPTIONAL,
        .size = sizeof (struct multiboot_header_tag_framebuffer),
        .width = FRAMEBUFFER_WIDTH,
        .height = FRAMEBUFFER_HEIGHT,
        .depth = FRAMEBUFFER_BPP
    },
#endif
    //GRUB 2尾
    .end = {
        .type = MULTIBOOT_HEADER_TAG_END,
        .flags = 0,
        .size = sizeof (struct multiboot_header_tag)
    }
};

//ELF 要求入口点
IMPL("elf")
//入口点 
void entry(void) {
    *(word*)(0xB8000) = 0x0C31;
    *(word*)(0xB8002) = 0x0C31;
    *(word*)(0xB8004) = 0x0C34;
    *(word*)(0xB8006) = 0x0C35;
    *(word*)(0xB8008) = 0x0C31;
    *(word*)(0xB800A) = 0x0C34;
    while (true);
}