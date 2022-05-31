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
#include <memory/pmm.h>
#include <memory/paging.h>
#include <tayhuang/paging.h>
#include <kheap.h>
#include <tayhuang/control_registers.h>

PUBLIC void load_segment(Elf64_Phdr *program, void *addr, void **start, void **limit) {
    char buffer[256];
    if (program->p_type == PT_LOAD) {
        *start = (void*)min ((qword)*start, program->p_vaddr);
        *limit = (void*)max ((qword)*limit, program->p_vaddr + program->p_memsz);
        linfo ("Segment type: LOAD");

        int pages_need = program->p_filesz / MEMUNIT_SZ + ((program->p_memsz % MEMUNIT_SZ) != 0);
        sprintk (buffer, "Require Pages: %d", pages_need);
        linfo (buffer);
        
        int sum = 0;
        while (sum < pages_need) {
            linfo ("*******");

            int num = 0;
            void *pages = find_freepages(pages_need - sum, &num);

            sprintk (buffer, "Pages found: addr = %P ; num = %d", pages, num);
            linfo (buffer);

            for (int i = 0 ; i < num ; i ++)
                mark_used(pages + i * MEMUNIT_SZ);

            void *start_vaddr = program->p_vaddr + sum * MEMUNIT_SZ;
            set_mapping(start_vaddr, pages, num, true, false);

            sprintk (buffer, "Mapped %P~%P to %P~%P", start_vaddr, start_vaddr + num * MEMUNIT_SZ, pages, pages + num * MEMUNIT_SZ);
            linfo (buffer);

            int copy_num = min(program->p_filesz - sum * MEMUNIT_SZ, num * MEMUNIT_SZ);
            void *start_file = addr + program->p_offset + sum * MEMUNIT_SZ;
            memcpy(pages, start_file, copy_num);
            
            sprintk (buffer, "Copied %P to %P, count = %dB", start_file, pages, copy_num);
            linfo (buffer);
            
            sum += num;
        }
    }
}

#define STACK_PAGES (64)

PUBLIC void* alloc_stack(void *entry) {
    linfo ("------------");
    linfo ("Allocing Stack");

    char buffer[256];

    int pages_need = STACK_PAGES;
    int sum = 0;

    while (sum < pages_need) {
        linfo ("*******");

        int num = 0;
        void *pages = find_freepages(pages_need - sum, &num);
        sprintk (buffer, "Pages found: addr = %P ; num = %d", pages, num);
        linfo (buffer);

        for (int i = 0 ; i < num ; i ++)
            mark_used(pages + i * MEMUNIT_SZ);

        void *start_vaddr = entry - sum * MEMUNIT_SZ - num * MEMUNIT_SZ;
        set_mapping(start_vaddr, pages, num, true, false);

        sprintk (buffer, "Mapped %P~%P to %P~%P", start_vaddr, start_vaddr + num * MEMUNIT_SZ, pages, pages + num * MEMUNIT_SZ);
        linfo (buffer);

        sum += num;
    }

    return entry - STACK_PAGES * MEMUNIT_SZ; 
}

PUBLIC void mapping_kernel(void);

PUBLIC program_info load_kmod_from_memory(void *addr) {
    Elf64_Ehdr *elf_head = (Elf64_Ehdr*)addr;
    char buffer[256];

    sprintk(buffer, "ELF Magic: %#02X %#02X %#02X %#02X",
     elf_head->e_ident[0], elf_head->e_ident[1], elf_head->e_ident[2], elf_head->e_ident[3]);
    linfo (buffer);

    sprintk(buffer, "Entrypoint: %P", elf_head->e_entry);
    linfo (buffer);

    void *pgd = create_pgd();
    set_pml4(pgd);

    void *start = (void*)0xFFFFFFFFFFFFFFFF, *limit = NULL;

    for (int i = 0 ; i < elf_head->e_phnum ; i ++) {
        linfo ("------------------------------------");
        sprintk (buffer, "Segment %d", i);
        linfo (buffer);

        Elf64_Phdr *program = (Elf64_Phdr*)(addr + elf_head->e_phoff + i * elf_head->e_phentsize);

        load_segment (program, addr, &start, &limit);
    }

    void *bottom = alloc_stack(elf_head->e_entry);
    mapping_kernel();

    program_info info = {
        .entry = elf_head->e_entry,
        .start = start,
        .limit = limit,
        .pgd = pgd,
        .stack_top = elf_head->e_entry,
        .stack_bottom = bottom
    };
    return info;
}