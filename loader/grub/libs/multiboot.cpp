/**
 * @file multiboot.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief Multiboot
 * @version alpha-1.0.0
 * @date 2022-10-22
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <libs/multiboot2.h>
#include <tayhuang/attributes.h>

/**
 * @brief Tayhuang OS GRUB 2 Boot Loader 程序头结构
 * 应GRUB2要求实现
 * 
 */
IMPL struct tayhuang_header {
    /**
     * @brief Mulitiboot2 头
     * 
     */
    struct multiboot_header header;
    //若启用VBE
#ifdef VBE_ENABLE
    /**
     * @brief 屏幕信息
     * 
     */
    struct multiboot_header_tag_framebuffer framebuffer;
    /**
     * @brief 保留位 仅供对齐
     * 
     */
    multiboot_uint32_t reserved0;
#endif
    /**
     * @brief Mulitiboot2 尾巴
     * 
     */
    struct multiboot_header_tag end;
} 
//按字节对齐
__attribute__((packed));

/**
 * @brief Tayhuang OS GRUB 2 Boot Loader 程序头
 * 应GRUB2要求实现
 * 放在.multiboot段下
 */
IMPL struct tayhuang_header TAYHUANG_HEADER 
//将这个头放在.multiboot段下
__attribute__((section(".multiboot")))
= {
    // Multiboot 2 头
    .header = {
        .magic    = MULTIBOOT2_HEADER_MAGIC,
        .architecture = MULTIBOOT_ARCHITECTURE_I386,
        .header_length = sizeof (struct tayhuang_header),
        .checksum = -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + sizeof(struct tayhuang_header)),
    },
    //启用VBE
#ifdef VBE_ENABLE
    /**
     * @brief 屏幕宽
     * 
     */
    #define FRAMEBUFFER_WIDTH 1024
    /**
     * @brief 
     * 
     */
     * 
     */
    #define FRAMEBUFFER_HEIGHT 768
    /**
     * @brief 像素位深
     * 
     */
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
    // Multiboot 2 尾
    .end = {
        .type = MULTIBOOT_HEADER_TAG_END,
        .flags = 0,
        .size = sizeof (struct multiboot_header_tag)
    }
};