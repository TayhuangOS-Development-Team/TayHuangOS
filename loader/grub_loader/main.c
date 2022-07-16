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
#include <logging.h>

#include <lheap.h>

#include <disk.h>
#include <fs/fat32.h>

#include <int_handlers.h>

#include <lm/setup_lm.h>

#include <printf.h>
#include <show_icon.h>

#include <info_parser.h>
#include <init.h>

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

//loader主函数
PUBLIC void loader_main(struct multiboot_tag *multiboot_info) {
    init_gdt();

    init_idt();
    init_pic();

    char buffer[256];

    init_serial();

    sprintf (buffer, "Hello, OS World!I'm \"%s\"", "Tayhuang OS Grub Loader");
    linfo ("Loader", buffer);
    sprintf (buffer, "Loading Kernel and Setup Module Now...");
    linfo ("Loader", buffer);

    asmv ("finit");

    init_lheap(0x800000);

    parse_result_struct result;

    parse_args(multiboot_info, &result);

    asmv ("sti");

    void *framebuffer = result.framebuffer;
    int width = result.screen_width;
    int height = result.screen_height;

#ifdef VBE_ENABLE
    show_icon(framebuffer, width, height);
#endif

    goto_longmode(7 << 3, result.memsz, result.memsz_high, result.is_graphic, width, height, framebuffer);
}

//loader入口点
PUBLIC void entry(void) {
    register int magic __asm__("eax"); //Loader 魔数 存放在eax
    register struct multiboot_tag *multiboot_info __asm__("ebx"); //multiboot info 存放在ebx

    asmv ("movl $0x1008000, %esp");

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) { //魔数不匹配
        while (true);
    }

    loader_main(multiboot_info); //进入loader主函数

    while (1); //jmp $
}