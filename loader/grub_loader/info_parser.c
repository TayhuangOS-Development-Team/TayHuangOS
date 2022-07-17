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
 * info_parser.c
 * 
 * 解析multiboot2参数
 * 
 */



#include <info_parser.h>

#include <printf.h>

PRIVATE void parse_mem_info(struct multiboot_tag_mmap *tag, qword *memsz) {
    int size = tag->size - 4 * 4;
    int entry_num = size / tag->entry_size;

    *memsz = 0;

    for (int i = 0 ; i < entry_num ; i ++) {
        if (tag->entries[i].type == MULTIBOOT_MEMORY_AVAILABLE) {
            *memsz = max(*memsz, (dword)tag->entries[i].addr + (dword)tag->entries[i].len);
        }
    }

    *memsz += 0x20000; //最终结果会少128KB
}

PRIVATE void parse_vbe(struct multiboot_tag_vbe *tag) {
}

PRIVATE void parse_framebuffer(struct multiboot_tag_framebuffer *tag, void **framebuffer, int *width, int *height) {
    *framebuffer = (void*)(tag->common.framebuffer_addr & 0xFFFFFFFF);
    *width = tag->common.framebuffer_width;
    *height = tag->common.framebuffer_height;
}

PUBLIC void parse_args(struct multiboot_tag *tag, parse_result_struct *result) {
    tag = ((void*)tag) + 8;
    while (tag->type != MULTIBOOT_TAG_TYPE_END) {
        switch (tag->type)
        {
        case MULTIBOOT_TAG_TYPE_MMAP: {
            parse_mem_info((struct multiboot_tag_mmap*)tag, &result->memsz);
            break;
        }
        case MULTIBOOT_TAG_TYPE_VBE: {
            parse_vbe((struct multiboot_tag_vbe*)tag);
            break;
        }
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
            parse_framebuffer((struct multiboot_tag_framebuffer*)tag, &result->framebuffer, &result->screen_width, &result->screen_height);
            break;
        }
        default: {
            break;
        }
        }

        int tag_size = (tag->size + 7) & ~7;
        tag = (struct multiboot_tag*)(((void*)tag) + tag_size);
    }

#ifndef VBE_ENABLE
    result->is_graphic = false;
    result->screen_width = 80;
    result->screen_height = 25;
    result->framebuffer = (void*)0xB8000;
#else
    result->is_graphic = true;
#endif
}