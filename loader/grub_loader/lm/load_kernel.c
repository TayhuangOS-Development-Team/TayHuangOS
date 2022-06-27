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
 * load_kernel.c
 * 
 * 加载内核
 * 
 */


#include "load_kernel.h"
#include <fs/common.h>
#include <disk.h>
#include <printf.h>
#include <lheap.h>
#include <elf.h>
#include <string.h>

PUBLIC void load_program(void *kernel_bin, Elf64_Phdr *program_header, void **start, void **limit) {
    if (program_header->p_type != PT_LOAD) //不是要被加载的段
        return;
    memcpy(program_header->p_vaddr, kernel_bin + program_header->p_offset, program_header->p_filesz);
    memset(program_header->p_vaddr + program_header->p_filesz, 0, program_header->p_memsz - program_header->p_filesz);
    *limit = program_header->p_vaddr + program_header->p_memsz;
    *start = program_header->p_vaddr;
}

void *load_elf(void *kernel_bin, void **kernel_start, void **kernel_limit) {
    Elf64_Ehdr *elf_header = kernel_bin;
    for (int i = 0 ; i < elf_header->e_phnum ; i ++) {
        void *plimit = NULL;
        void *pstart = NULL;
        load_program(kernel_bin, kernel_bin + elf_header->e_phoff + i * elf_header->e_phentsize, &pstart, &plimit);
        *kernel_limit = max(*kernel_limit, plimit);
        *kernel_start = max(*kernel_start, pstart);
    }
    return elf_header->e_entry;
}

void load_kernel(load_result_struct *result) {
    void *kernel_bin_buffer = lmalloc(KERNEL_BIN_SIZE);
    void *setup_mod_buffer = lmalloc(SETUP_MOD_SIZE);
    for (int i = 0 ; i < 4 ; i ++) {
        fs_context context = load_fs(DISK_SEL_IDE0_MASTER, 0);

        if (context == NULL)
            continue;

        bool status = load_file(context, "kernel.bin", kernel_bin_buffer, true);

        status &= load_file(context, "setup.mod", setup_mod_buffer, true);

        if (! status) {
            terminate_fs(context);
            continue;
        }

        result->setup_mod = setup_mod_buffer;

        result->kernel_limit = (void*)0xFFFFFFFF;
        result->kernel_entry = load_elf(kernel_bin_buffer, &result->kernel_start, &result->kernel_limit);

        terminate_fs(context);

        result->status = true;
        return;
    }

    lfree (kernel_bin_buffer);
    lfree (setup_mod_buffer);

    printf ("Couldn't load kernel!");
    result->status = false;
    result->setup_mod = NULL;
    result->kernel_entry = NULL;
    result->kernel_start = NULL;
    result->kernel_limit = NULL;
}