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
 * kmod_loader.c
 *
 * 内核模块加载器
 *
 */



#include "kmod_loader.h"
#include <elf.h>

#include <debug/logging.h>
#include <display/printk.h>

PUBLIC void load_kmod_from_memory(void *addr) {
    Elf64_Ehdr *elf_head = (Elf64_Ehdr*)addr;
    char buffer[512];

    sprintk(buffer, "ELF Magic: %#02X %#02X %#02X %#02X",
     elf_head->e_ident[0], elf_head->e_ident[1], elf_head->e_ident[2], elf_head->e_ident[3]);
    linfo (buffer);

    sprintk(buffer, "Entrypoint: %P", elf_head->e_entry);
    linfo (buffer);
    while (true);
}