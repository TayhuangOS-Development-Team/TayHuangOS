// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/pm/load_stage3.c
 *
 * Stage3 is loaded here
 */



#include "entry.h"
#include <elf.h>
#include "../drivers/disk/disk_driver.h"
#include "../drivers/drivers.h"
#include "../printf.h"

PRIVATE void copy_from(sreg_t segment, addr_t offset, void* dst, int size) {
    stfs(segment);
    for (int i = 0 ; i < size ; i ++) {
        *(char*)(dst + i) = rdfs8(offset + i);
    }
}


PRIVATE void copy_from_to(const sreg_t src_seg, const addr_t src, const sreg_t dst_seg, const addr_t dst, const int size) {
    stfs(src_seg);
    stes(dst_seg);
    for (int i = 0 ; i < size ; i ++) {
        byte v = rdfs8(src + i);
        stes8(dst + i, v);
    }
}

PRIVATE void load_stage3_programs(Elf32_Off offset) {
    Elf32_Phdr program_header;
    copy_from(STAGE3_BIN_SEGMENT, STAGE3_BIN_OFFSET + offset, &program_header, sizeof(Elf32_Phdr));
    if (program_header.p_type != PT_LOAD)
        return;
    copy_from_to(STAGE3_BIN_SEGMENT, program_header.p_offset, STAGE3_SEGMENT, program_header.p_vaddr, program_header.p_filesz);
}

PRIVATE void* load_stage3_elf(void) {
    Elf32_Ehdr elf_header;
    copy_from(STAGE3_BIN_SEGMENT, STAGE3_BIN_OFFSET + 0, &elf_header, sizeof(Elf32_Ehdr));
    for (int i = 0 ; i < elf_header.e_phnum ; i ++) {
        load_stage3_programs(elf_header.e_phoff + i * elf_header.e_phentsize);
    }
    return (void*)elf_header.e_entry;
}

PUBLIC void* load_stage3(void) {
    APACK(dk, load_file) pack;
    pack.name = "STAGE3  BIN";
    pack.offset = STAGE3_BIN_OFFSET;
    pack.segment = STAGE3_BIN_SEGMENT;
    if (! a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_LOAD_FILE, &pack)) {
        return NULL;
    }
    void* ptr = load_stage3_elf();
    return ptr;
}