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

#include <init/gdt.h>
#include <init/intterup.h>

#include <logging.h>

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
     * @brief 屏幕高
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

/**
 * @brief GRUB2 Loader 主函数
 * 
 * @param multiboot_info multiboot info
 */
IMPL NORETURN void main(struct multiboot_tag *multiboot_info) {
    LINFO ("GRUB2 Loader", "Initialized");

    while (true);
}

/**
 * @brief GRUB 2 Loader 入口点
 * 
 */
IMPL NORETURN void entry(void) {
    register int magic __asm__("eax"); //Loader 魔数 存放在eax
    struct multiboot_tag *multiboot_info; //multiboot info 存放在ebx
    asmv ("movl %%ebx, %0" : "=g"(multiboot_info));

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) { //魔数不匹配
        while (true);
    }

    // 设置栈指针
    asmv ("movl $0x2008000, %esp");

    init_gdt();
    init_idt();
    init_pic();

    asmv("sti");

    init_serial();
    
    main(multiboot_info);
}