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



#include "libs/multiboot2.h"
#include "printf.h"
#include "init.h"

//Tayhuang OS GRUB Loader Multiboot2 header struct
struct tayhuang_header {
    struct multiboot_header header;
#ifdef VBE_ENABLE
    struct multiboot_header_tag_framebuffer framebuffer;
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
    //FIXME: Error here
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

//loader主函数
void loader_main(void *multiboot_info) {
    asmv ("finit");

    init_gdt();

    #ifndef VBE_ENABLE
        printf ("Hello, OS World!I'm \"%s\"\n", "Tayhuang OS Grub Loader");
        printf ("Loading Kernel and Setup Module Now...\n");
    #endif

    init_idt();
    init_pic();
    
    asmv ("sti");
}

//loader入口点
void entry(void) {
    register int magic __asm__("eax"); //Loader 魔数 存放在eax
    register void *multiboot_info __asm__("ebx"); //multiboot info 存放在ebx

    asmv ("movl $0x1008000, %esp");

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) { //魔数不匹配
        while (true);
    }

    loader_main(multiboot_info); //进入loader主函数

    while (1); //jmp $
}