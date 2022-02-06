/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * load_kernel.c
 *
 * 加载内核
 *
 */



#include "load_kernel.h"

#include "../fs/fat32.h"
#include "../disk.h"
#include "../memory.h"
#include "../printf.h"
#include <tayboot/elf.h>

#define KERNEL_BIN_ADDRESS (0x320000)

void load_program(Elf64_Phdr *program_header, void **start, void **limit) {
    printf ("{p_type: %d, p_vaddr: %#8X, p_offset: %#8X, p_filesz: %#8X}\n",
            program_header->p_type, program_header->p_vaddr, program_header->p_offset, program_header->p_filesz);
    if (program_header->p_type != PT_LOAD) //不是要被加载的段
        return;
    memcpy(program_header->p_vaddr, KERNEL_BIN_ADDRESS + program_header->p_offset, program_header->p_filesz);
    *start = program_header->p_vaddr;
    *limit = program_header->p_vaddr + program_header->p_filesz;
}

void *load_elf(void **kernel_start, void **kernel_limit) {
    Elf64_Ehdr *elf_header = KERNEL_BIN_ADDRESS;
    printf ("e_type: %d\n", elf_header->e_type);
    printf ("e_machine: %d\n", elf_header->e_machine);
    printf ("e_version: %d\n", elf_header->e_version);
    printf ("e_entry: %#8x\n", elf_header->e_entry);
    printf ("e_phoff: %#8X\n", elf_header->e_phoff);
    printf ("e_shoff: %#8X\n", elf_header->e_shoff);
    printf ("e_phentsize: %d\n", elf_header->e_phentsize);
    printf ("e_phnum: %d\n", elf_header->e_phnum);
    printf ("e_shentsize: %d\n", elf_header->e_shentsize);
    printf ("e_shnum: %d\n", elf_header->e_shnum);
    printf ("e_ehsize: %d\n", elf_header->e_ehsize);
    for (int i = 0 ; i < elf_header->e_phnum ; i ++) {
        void *pstart, *plimit;
        load_program(KERNEL_BIN_ADDRESS + elf_header->e_phoff + i * elf_header->e_phentsize, &pstart, &plimit);
        *kernel_start = min(*kernel_start, pstart);
        *kernel_limit = max(*kernel_limit, plimit);
    }
    return elf_header->e_entry;
}

void *load_kernel(void** kernel_start, void** kernel_limit) {
    void *context = get_context(DISK_SEL_IDE1_MASTER);
    bool success = loadfile(context, "kernel.bin", KERNEL_BIN_ADDRESS);
    terminate_fs_context(context);
    if (! success) {
        return NULL;
    }
    return load_elf(kernel_start, kernel_limit);
}