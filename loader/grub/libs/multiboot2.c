/**
 * @file multiboot2.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief Multiboot2
 * @version alpha-1.0.0
 * @date 2022-12-31
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#include <tay/types.h>

#include <multiboot2.h>

/**
 * @brief Tayhuang OS GRUB 2 Boot Loader 程序头结构
 * 应GRUB2要求实现
 */
struct tayhuang_header {
    /** Mulitiboot2 头 */
    struct multiboot_header header;
    //若启用VBE
#ifdef VBE_ENABLE
    /** 屏幕信息 */
    struct multiboot_header_tag_framebuffer framebuffer;
    /** 保留位 仅供对齐 */
    multiboot_uint32_t reserved0;
#endif
    /** Mulitiboot2 尾 */
    struct multiboot_header_tag end;
}
//按字节对齐
__attribute__((packed));

/** 屏幕宽 */
#define FRAMEBUFFER_WIDTH 1024

/** 屏幕高 */
#define FRAMEBUFFER_HEIGHT 768

/** 像素位深 */
#define FRAMEBUFFER_BPP 24

/** 架构 */
#define ARCHITECTURE MULTIBOOT_ARCHITECTURE_I386

/** 校验码 */
#define CHECKSUM -(MULTIBOOT2_HEADER_MAGIC + ARCHITECTURE + sizeof(struct tayhuang_header))

/**
 * @brief Tayhuang OS GRUB 2 Boot Loader 程序头
 * 应GRUB2要求实现
 * 放在.multiboot段下
 */
struct tayhuang_header TAYHUANG_HEADER
//将这个头放在.multiboot段下
__attribute__((section(".multiboot")))
= {
    // Multiboot 2 头
    .header = {
        .magic    = MULTIBOOT2_HEADER_MAGIC,
        .architecture = ARCHITECTURE,
        .header_length = sizeof (struct tayhuang_header),
        .checksum = CHECKSUM
    },
    //启用VBE
#ifdef VBE_ENABLE
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
    // Multiboot 2 尾
    .end = {
        .type = MULTIBOOT_HEADER_TAG_END,
        .flags = 0,
        .size = sizeof (struct multiboot_header_tag)
    }
};