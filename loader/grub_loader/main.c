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
#include <logging.h>
#include <tayhuang/ports.h>
#include <tayhuang/io.h>

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

#ifdef VBE_ENABLE

PRIVATE volatile int progress = -1;
PRIVATE volatile int cnt = 0;

#define CLOCKS_PER_SECOND (100)
#define ANIMATION_TIME (20)

PUBLIC void clock_irq_handler(int irq) {
    if (progress != -1 && progress < 100) {
        cnt ++;
        if (cnt >= (ANIMATION_TIME * 100 / CLOCKS_PER_SECOND)) {
            cnt = 0;
            progress ++;
        }
    }
}

#define PIT_FREQUENCY (1193181.6666f)

#define ICON_SIZE (1028 * 1024)

PUBLIC bool init_pit(float frequency) { //初始化PIT
    if (frequency > PIT_FREQUENCY) //频率过高
        return false;

    int count = (int)(PIT_FREQUENCY / frequency);
    if ((PIT_FREQUENCY - count * frequency) > (frequency / 2))
        count ++;

    if (count >= 65535) return false; //频率过低

    outb(PIT_CHANNEL0, (byte)count); //设置频率
    outb(PIT_CHANNEL0, (byte)(count >> 8)); //设置频率

    return true;
}

void clear_screen(void *framebuffer, int width, int height) {
    for (int i = 0 ; i < height ; i ++) {
        for (int j = 0 ; j < width ; j ++) {
            *(char*)(framebuffer + (i * width + j) * 3 + 0) = 0xFF;
            *(char*)(framebuffer + (i * width + j) * 3 + 1) = 0xFF;
            *(char*)(framebuffer + (i * width + j) * 3 + 2) = 0xFF;
        }
    }
}

void display_icon(void *icon, void *framebuffer, int width, int height) {
    asmv ("cli");

    int ic_width = *(int*)icon;
    int ic_height = *(int*)(icon + 4);
    icon += 8;

    //居中
    int offset_x = width / 2 - ic_width / 2;
    int offset_y = height / 2 - ic_height / 2;

    for (int i = 0 ; i < ic_height ; i ++) {
        for (int j = 0 ; j < ic_width ; j ++) {
            dword __r =  *(byte*)(icon + (i * ic_width + j) * 4 + 0);
            dword __g =  *(byte*)(icon + (i * ic_width + j) * 4 + 1);
            dword __b =  *(byte*)(icon + (i * ic_width + j) * 4 + 2);
            dword gray = (__r * 30 + __g * 59 + __b * 11 + 50) / 100;

            dword r = __r * progress / 100 + gray * (100 - progress) / 100;
            dword g = __g * progress / 100 + gray * (100 - progress) / 100;
            dword b = __b * progress / 100 + gray * (100 - progress) / 100;

            *(char*)(framebuffer + ((i + offset_y) * width + j + offset_x) * 3 + 0) = b;
            *(char*)(framebuffer + ((i + offset_y) * width + j + offset_x) * 3 + 1) = g;
            *(char*)(framebuffer + ((i + offset_y) * width + j + offset_x) * 3 + 2) = r;
        }
    }

    asmv ("sti");
}

#endif

//loader主函数
PUBLIC void loader_main(struct multiboot_tag *multiboot_info) {
    asmv ("finit");

    init_gdt();

    char buffer[256];

    sprintf (buffer, "Hello, OS World!I'm \"%s\"", "Tayhuang OS Grub Loader");
    linfo ("Loader", buffer);
    sprintf (buffer, "Loading Kernel and Setup Module Now...");
    linfo ("Loader", buffer);

    init_idt();
    init_pic();

    init_serial();

    init_lheap(0x800000);

    parse_result_struct result;

    parse_args(multiboot_info, &result);

    asmv ("sti");

    void *framebuffer = result.framebuffer;
    int width = result.screen_width;
    int height = result.screen_height;

#ifdef VBE_ENABLE
    progress = 0;

    init_pit(CLOCKS_PER_SECOND);

    register_irq_handler(0, clock_irq_handler);
    enable_irq(0);

    clear_screen(framebuffer, width, height);

    fs_context ctx = load_fs(DISK_SEL_IDE0, 0);

    void *icon = lmalloc(ICON_SIZE);

    load_file(ctx, "tayicon.raw", icon, false);

    terminate_fs(ctx);

    volatile int last_progress = -1;
    while (last_progress < 100) {
        if (last_progress < progress)
            display_icon(icon, framebuffer, width, height);
        last_progress = progress;
    }

    lfree (icon);
#endif

    goto_longmode(7 << 3, result.memsz, result.memsz_high, result.is_graphic, result.screen_width, result.screen_height, result.framebuffer);
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