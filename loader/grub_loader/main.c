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
#include "lheap.h"
#include "disk.h"
#include "fs/fat32.h"

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

#define KERNEL_BIN_SIZE (256 * 1024)

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

    init_lheap(0x400000);

    partition_member members[4];

    get_partition(DISK_SEL_IDE0_MASTER, 0, &members[0]);
    get_partition(DISK_SEL_IDE0_MASTER, 1, &members[1]);
    get_partition(DISK_SEL_IDE0_MASTER, 2, &members[2]);
    get_partition(DISK_SEL_IDE0_MASTER, 3, &members[3]);

    for (int i = 0 ; i < 4 ; i ++) {
        printf ("Partition %d: Start LBA=%d, Sector Number = %d, Bootable = %s\n",
            i, members[i].start_lba, members[i].sector_number,
            members[i].state == PS_BOOTABLE ? "true" : "false");
    }

    fs_context ctx = load_fat32_fs(DISK_SEL_IDE0_MASTER, 0);

    void *kernel_bin_buffer = lmalloc(KERNEL_BIN_SIZE);
    if (! load_fat32_file(ctx, "kernel.bin", kernel_bin_buffer, true)) {
        printf ("Load kernel failed!\n");
    }

    terminate_fat32_fs(ctx);
    
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