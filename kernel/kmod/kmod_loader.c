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

#include <string.h>
#include <debug/logging.h>
#include <display/printk.h>
#include <memory/pmm.h>
#include <memory/paging.h>
#include <tayhuang/paging.h>
#include <kheap.h>
#include <tayhuang/control_registers.h>

PUBLIC void load_segment(Elf64_Phdr *program, void *addr, void **start, void **end) { //加载一个段
    char buffer[256];
    if (program->p_type == PT_LOAD) {
        *start = min((qword)*start, program->p_vaddr);
        *end = max((qword)*end, program->p_vaddr + program->p_memsz);

        linfo ("KMod Loader", "Segment type: LOAD");


        sprintk (buffer, "Segment %P~%P", program->p_vaddr, program->p_vaddr + program->p_memsz);
        linfo ("KMod Loader", buffer);

        int require_pages = (program->p_memsz / MEMUNIT_SZ) + (program->p_memsz % MEMUNIT_SZ != 0);

        sprintk (buffer, "Require pages = %d ; Require to copy %d bytes", require_pages, program->p_filesz);
        linfo ("KMod Loader", buffer);

        int remain_copy = program->p_filesz;

        int sum = 0;
        while (sum < require_pages) {
            int num = 0;

            void *pages = find_freepages(require_pages - sum, &num); //寻找空闲页
            sprintk (buffer, "Pages found: addr = %P ; num = %d", pages, num);
            linfo ("KMod Loader", buffer);

            for (int i = 0 ; i < num ; i ++)
                mark_used(pages + i * MEMUNIT_SZ); //设置被使用

            void *start_vaddr = (void*)(program->p_vaddr + sum * MEMUNIT_SZ);
            set_mapping(start_vaddr, pages, num, true, false); //映射

            sprintk (buffer, "Mapped %P~%P to %P~%P", start_vaddr, start_vaddr + num * MEMUNIT_SZ, pages, pages + num * MEMUNIT_SZ);
            linfo ("KMod Loader", buffer);

            if (remain_copy > 0) {
                int num_copy = (remain_copy < num * MEMUNIT_SZ) ? remain_copy : num * MEMUNIT_SZ;

                void *src = addr + program->p_offset + (program->p_filesz - remain_copy);

                memcpy(pages, src, num_copy);

                sprintk (buffer, "Copied %P to %P, count = %dB", src, pages, num_copy);
                linfo ("KMod Loader", buffer);

                if (num_copy < num * MEMUNIT_SZ) {
                    memset (pages + num_copy, 0, num * MEMUNIT_SZ - num_copy);
                }
            }
            else {
                memset (pages, 0, num * MEMUNIT_SZ);
            }

            sum += num;
        }
    }
}

#define STACK_PAGES (32)

PUBLIC void* alloc_stack(void *stack_top) { //分配栈
    linfo ("KMod Loader", "------------");
    linfo ("KMod Loader", "Allocing Stack");

    char buffer[256];

    int require_pages = STACK_PAGES;
    int sum = 0;

    while (sum < require_pages) {
        linfo ("KMod Loader", "*******");

        int num = 0;
        void *pages = find_freepages(require_pages - sum, &num); //寻找空闲页
        sprintk (buffer, "Pages found: addr = %P ; num = %d", pages, num);
        linfo ("KMod Loader", buffer);

        for (int i = 0 ; i < num ; i ++)
            mark_used(pages + i * MEMUNIT_SZ); //设置被使用

        void *start_vaddr = stack_top - sum * MEMUNIT_SZ - num * MEMUNIT_SZ;
        set_mapping(start_vaddr, pages, num, true, false); //映射

        sprintk (buffer, "Mapped %P~%P to %P~%P", start_vaddr, start_vaddr + num * MEMUNIT_SZ, pages, pages + num * MEMUNIT_SZ);
        linfo ("KMod Loader", buffer);

        sum += num;
    }

    return stack_top - STACK_PAGES * MEMUNIT_SZ;
}

#define HEAP_PAGES (16)

PUBLIC void* alloc_heap(void *heap_bottom) {
    linfo ("KMod Loader", "------------");
    linfo ("KMod Loader", "Allocing Heap");

    char buffer[256];

    int require_pages = HEAP_PAGES;
    int sum = 0;

    while (sum < require_pages) {
        linfo ("KMod Loader", "*******");

        int num = 0;
        void *pages = find_freepages(require_pages - sum, &num); //寻找空闲页
        sprintk (buffer, "Pages found: addr = %P ; num = %d", pages, num);
        linfo ("KMod Loader", buffer);

        for (int i = 0 ; i < num ; i ++)
            mark_used(pages + i * MEMUNIT_SZ);

        void *start_vaddr = heap_bottom + sum * MEMUNIT_SZ;
        set_mapping(start_vaddr, pages, num, true, false); //映射

        sprintk (buffer, "Mapped %P~%P to %P~%P", start_vaddr, start_vaddr + num * MEMUNIT_SZ, pages, pages + num * MEMUNIT_SZ);
        linfo ("KMod Loader", buffer);

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
    linfo ("KMod Loader", buffer);

    sprintk(buffer, "Entrypoint: %P", elf_head->e_entry);
    linfo ("KMod Loader", buffer);

    void *pgd = create_pgd(); //创建页目录
    set_pml4(pgd);

    void *start = (void*)0xFFFFFFFFFFFFFFFF, *end = NULL;

    for (int i = 0 ; i < elf_head->e_phnum ; i ++) { //加载段
        linfo ("KMod Loader", "------------------------------------");
        sprintk (buffer, "Segment %d", i);
        linfo ("KMod Loader", buffer);

        Elf64_Phdr *program = (Elf64_Phdr*)(addr + elf_head->e_phoff + i * elf_head->e_phentsize);

        load_segment (program, addr, &start, &end);
    }

    void *stack_bottom = alloc_stack(start); //分配栈

    qword end_page = ((qword)end) / MEMUNIT_SZ + 1;
    void *heap_bottom = (end_page + 1) * MEMUNIT_SZ;
    void *heap_top = alloc_heap(heap_bottom); //分配堆

    mapping_kernel(); //将内核映射到内存中

    program_info info = {
        .entry = elf_head->e_entry,
        .start = start,
        .end = end,
        .pgd = pgd,
        .stack_top = start,
        .stack_bottom = stack_bottom,
        .heap_bottom = heap_bottom,
        .heap_top = heap_top
    };
    return info;
}