// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/pm/load_stage3.c
 *
 * 加载stage3
 */



#include "entry.h"
#include <elf.h>
#include "../drivers/disk/disk_driver.h"
#include "../drivers/drivers.h"
#include "../printf.h"

PRIVATE void copy_from(sreg_t segment, addr_t offset, void* dst, int size) { //从...拷贝
    stfs(segment);
    for (int i = 0 ; i < size ; i ++) {
        *(char*)(dst + i) = rdfs8(offset + i);
    }
}

//从...拷贝到...
PRIVATE void copy_from_to(const sreg_t src_seg, const addr_t src, const sreg_t dst_seg, const addr_t dst, const int size) {
    stfs(src_seg);
    stes(dst_seg);
    for (int i = 0 ; i < size ; i ++) {
        byte v = rdfs8(src + i);
        stes8(dst + i, v);
    }
}

//加载一个段
PRIVATE void load_stage3_programs(Elf32_Off offset) {
    Elf32_Phdr program_header;
    copy_from(STAGE3_BIN_SEGMENT, STAGE3_BIN_OFFSET + offset, &program_header, sizeof(Elf32_Phdr)); //获取program header
    if (program_header.p_type != PT_LOAD) //不是要被加载的段
        return;
    copy_from_to(STAGE3_BIN_SEGMENT, program_header.p_offset, STAGE3_SEGMENT, program_header.p_vaddr, program_header.p_filesz); //拷贝
}

//加载stage3
PRIVATE void* load_stage3_elf(void) {
    Elf32_Ehdr elf_header;
    copy_from(STAGE3_BIN_SEGMENT, STAGE3_BIN_OFFSET + 0, &elf_header, sizeof(Elf32_Ehdr)); //获取elf头
    for (int i = 0 ; i < elf_header.e_phnum ; i ++) { //program header数
        load_stage3_programs(elf_header.e_phoff + i * elf_header.e_phentsize);
    }
    return (void*)elf_header.e_entry; //返回起始点
}

PUBLIC void* load_stage3(void) {
    APACK(dk, load_file) pack;
    pack.name = "STAGE3  BIN";
    pack.offset = STAGE3_BIN_OFFSET;
    pack.segment = STAGE3_BIN_SEGMENT;
    if (! a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_LOAD_FILE, &pack)) { //加载stage3.bin
        return NULL;
    }
    return load_stage3_elf();
}