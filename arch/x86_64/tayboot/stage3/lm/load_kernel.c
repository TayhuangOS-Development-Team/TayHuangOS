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

#define KERNEL_BIN_ADDRESS (0x600000)

void load_program(Elf64_Phdr *program_header, void **limit) {
    if (program_header->p_type != PT_LOAD) //不是要被加载的段
        return;
    memcpy(program_header->p_vaddr, KERNEL_BIN_ADDRESS + program_header->p_offset, program_header->p_filesz);
    *limit = program_header->p_vaddr + program_header->p_memsz;
}

void *load_elf(void **kernel_limit) {
    Elf64_Ehdr *elf_header = KERNEL_BIN_ADDRESS;
    for (int i = 0 ; i < elf_header->e_phnum ; i ++) {
        void *plimit = NULL;
        load_program(KERNEL_BIN_ADDRESS + elf_header->e_phoff + i * elf_header->e_phentsize, &plimit);
        *kernel_limit = max(*kernel_limit, plimit);
    }
    return elf_header->e_entry;
}

void *load_kernel(void** kernel_start, void** kernel_limit) {
    void *context = get_context(DISK_SEL_IDE1_MASTER);
    printf ("Loading kernel...\n");
    bool success = loadfile(context, "kernel.bin", KERNEL_BIN_ADDRESS);
    if (! success) {
        return NULL;
    }
    printf ("\nLoading setup...\n");
    success = loadfile(context, "setup.mod", SETUP_MOD_ADDRESS);
    if (! success) {
        return NULL;
    }
    terminate_fs_context(context);
    *kernel_limit = NULL;
    return *kernel_start = load_elf(kernel_limit);
}