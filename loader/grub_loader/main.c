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



#include <libs/multiboot2.h>
#include <printf.h>
#include <init.h>
#include <lheap.h>
#include <disk.h>
#include <fs/fat32.h>
#include <int_handlers.h>
#include <lm/setup_lm.h>
#include <info_parser.h>

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

#define ICON_SIZE (1028 * 1024)

//loader主函数
PUBLIC void loader_main(struct multiboot_tag *multiboot_info) {
    asmv ("finit");

    init_gdt();

    #ifndef VBE_ENABLE
        printf ("Hello, OS World!I'm \"%s\"\n", "Tayhuang OS Grub Loader");
        printf ("Loading Kernel and Setup Module Now...\n");
    #endif

    init_idt();
    init_pic();

    init_lheap(0x400000);

    parse_result_struct result;

    parse_args(multiboot_info, &result);

    asmv ("sti");

    partition_member members[4];

    get_partition(DISK_SEL_IDE0_MASTER, 0, &members[0]);
    get_partition(DISK_SEL_IDE0_MASTER, 1, &members[1]);
    get_partition(DISK_SEL_IDE0_MASTER, 2, &members[2]);
    get_partition(DISK_SEL_IDE0_MASTER, 3, &members[3]);

    for (int i = 0 ; i < 4 ; i ++) {
        printf ("Partition %d: Start LBA=%d, Sector Number = %d, Bootable = %s\n",
            i + 1, members[i].start_lba, members[i].sector_number,
            members[i].state == PS_BOOTABLE ? "true" : "false");
    }

    void *framebuffer = result.framebuffer;
    int width = result.screen_width;
    int height = result.screen_height;

    fs_context ctx = load_fs(DISK_SEL_IDE0, 0);

    void *icon = lmalloc(ICON_SIZE);

    load_file(ctx, "tayicon.raw", icon, false);

    for (int i = 0 ; i < height ; i ++) {
        for (int j = 0 ; j < width ; j ++) {
            *(char*)(framebuffer + (i * width + j) * 3 + 0) = 0xFF;
            *(char*)(framebuffer + (i * width + j) * 3 + 1) = 0xFF;
            *(char*)(framebuffer + (i * width + j) * 3 + 2) = 0xFF;
        }
    }


    int ic_width = *(int*)icon;
    int ic_height = *(int*)(icon + 4);
    icon += 8;

    int offset_x = result.screen_width / 2 - ic_width / 2;
    int offset_y = result.screen_height / 2 - ic_height / 2;

    for (int i = 0 ; i < ic_height ; i ++) {
        for (int j = 0 ; j < ic_width ; j ++) {
            *(char*)(framebuffer + ((i + offset_y) * width + j + offset_x) * 3 + 0) = *(char*)(icon + (i * ic_width + j) * 4 + 2);
            *(char*)(framebuffer + ((i + offset_y) * width + j + offset_x) * 3 + 1) = *(char*)(icon + (i * ic_width + j) * 4 + 1);
            *(char*)(framebuffer + ((i + offset_y) * width + j + offset_x) * 3 + 2) = *(char*)(icon + (i * ic_width + j) * 4 + 0);
        }
    }
    //goto_longmode(7 << 3, result.memsz, result.memsz_high, result.is_graphic, result.screen_width, result.screen_height, result.framebuffer);
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