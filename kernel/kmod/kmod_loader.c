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

PUBLIC void load_segment(Elf64_Phdr *program, void *addr, void **start, void **limit) { //加载一个段
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
            void *pages = find_freepages(pages_need - sum, &num); //寻找空闲页

            sprintk (buffer, "Pages found: addr = %P ; num = %d", pages, num);
            linfo (buffer);

            for (int i = 0 ; i < num ; i ++)
                mark_used(pages + i * MEMUNIT_SZ); //设置被使用

            void *start_vaddr = program->p_vaddr + sum * MEMUNIT_SZ;
            set_mapping(start_vaddr, pages, num, true, false); //进行映射

            sprintk (buffer, "Mapped %P~%P to %P~%P", start_vaddr, start_vaddr + num * MEMUNIT_SZ, pages, pages + num * MEMUNIT_SZ);
            linfo (buffer);

            int copy_num = min(program->p_filesz - sum * MEMUNIT_SZ, num * MEMUNIT_SZ);
            void *start_file = addr + program->p_offset + sum * MEMUNIT_SZ;
            memcpy(pages, start_file, copy_num); //进行复制

            sprintk (buffer, "Copied %P to %P, count = %dB", start_file, pages, copy_num);
            linfo (buffer);

            sum += num;
        }
    }
}

#define STACK_PAGES (32)

PUBLIC void* alloc_stack(void *stack_top) { //分配栈
    linfo ("------------");
    linfo ("Allocing Stack");

    char buffer[256];

    int pages_need = STACK_PAGES;
    int sum = 0;

    while (sum < pages_need) {
        linfo ("*******");

        int num = 0;
        void *pages = find_freepages(pages_need - sum, &num); //寻找空闲页
        sprintk (buffer, "Pages found: addr = %P ; num = %d", pages, num);
        linfo (buffer);

        for (int i = 0 ; i < num ; i ++)
            mark_used(pages + i * MEMUNIT_SZ); //设置被使用

        void *start_vaddr = stack_top - sum * MEMUNIT_SZ - num * MEMUNIT_SZ;
        set_mapping(start_vaddr, pages, num, true, false); //映射

        sprintk (buffer, "Mapped %P~%P to %P~%P", start_vaddr, start_vaddr + num * MEMUNIT_SZ, pages, pages + num * MEMUNIT_SZ);
        linfo (buffer);

        sum += num;
    }

    return stack_top - STACK_PAGES * MEMUNIT_SZ;
}

#define HEAP_PAGES (16)

PUBLIC void* alloc_heap(void *heap_bottom) {
    linfo ("------------");
    linfo ("Allocing Heap");

    char buffer[256];

    int pages_need = HEAP_PAGES;
    int sum = 0;

    while (sum < pages_need) {
        linfo ("*******");

        int num = 0;
        void *pages = find_freepages(pages_need - sum, &num); //寻找空闲页
        sprintk (buffer, "Pages found: addr = %P ; num = %d", pages, num);
        linfo (buffer);

        for (int i = 0 ; i < num ; i ++)
            mark_used(pages + i * MEMUNIT_SZ);

        void *start_vaddr = heap_bottom + sum * MEMUNIT_SZ;
        set_mapping(start_vaddr, pages, num, true, false); //映射

        sprintk (buffer, "Mapped %P~%P to %P~%P", start_vaddr, start_vaddr + num * MEMUNIT_SZ, pages, pages + num * MEMUNIT_SZ);
        linfo (buffer);

        sum += num;
    }

    return heap_bottom + HEAP_PAGES * MEMUNIT_SZ;
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

    void *pgd = create_pgd(); //创建页目录
    set_pml4(pgd);

    void *start = (void*)0xFFFFFFFFFFFFFFFF, *limit = NULL;

    for (int i = 0 ; i < elf_head->e_phnum ; i ++) { //加载段
        linfo ("------------------------------------");
        sprintk (buffer, "Segment %d", i);
        linfo (buffer);

        Elf64_Phdr *program = (Elf64_Phdr*)(addr + elf_head->e_phoff + i * elf_head->e_phentsize);

        load_segment (program, addr, &start, &limit);
    }

    void *stack_bottom = alloc_stack(start); //分配栈

    qword limit_page = ((qword)limit) / MEMUNIT_SZ + 1;
    void *heap_bottom = (limit_page + 1) * MEMUNIT_SZ;
    void *heap_top = alloc_heap(heap_bottom); //分配堆

    mapping_kernel(); //将内核映射到内存中

    program_info info = {
        .entry = elf_head->e_entry,
        .start = start,
        .limit = limit,
        .pgd = pgd,
        .stack_top = start,
        .stack_bottom = stack_bottom,
        .heap_bottom = heap_bottom,
        .heap_top = heap_top
    };
    return info;
}