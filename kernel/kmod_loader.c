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
 * kmod_loader.c
 * 
 * 内核模块加载器
 * 
 */



#include <kmod_loader.h>
#include <global.h>
#include <logging.h>

#include <elf.h>

#include <memory/pmm.h>
#include <memory/paging.h>

#include <tayhuang/kmod_prototypes.h>

PUBLIC void load_program(void *pgd, void *addr, Elf64_Phdr *header, void **start, void **end) {
    if (header->p_type != PT_LOAD)
        return;
    int pages = (header->p_memsz + (MEMUNIT_SZ - 1)) / MEMUNIT_SZ;
    void *start_page_addr = (header->p_vaddr + (MEMUNIT_SZ - 1)) & ~(MEMUNIT_SZ - 1);

    linfo ("KMod Loader", "%P %d", start_page_addr, pages);

    alloc_vpages(pgd, start_page_addr, pages);

    //复制
    pvmemcpy(pgd, header->p_vaddr, addr + header->p_offset, header->p_filesz);

    //剩下清0
    vmemset (pgd, header->p_vaddr + header->p_filesz, 0, header->p_memsz - header->p_filesz);

    *start = (void*)(header->p_vaddr);
    *end   = (void*)(header->p_vaddr + header->p_memsz);
}

#define STACK_SIZE (16 * MEMUNIT_SZ)

PUBLIC void alloc_stack(void *pgd, void *stack_top, void **stack_bottom) {
    *stack_bottom = stack_top - STACK_SIZE;
    alloc_vpages(pgd, *stack_bottom, STACK_SIZE / MEMUNIT_SZ);
}

#define HEAP_SIZE (16 * MEMUNIT_SZ)

PUBLIC void alloc_heap(void *pgd, void *heap_bottom, void **heap_top) {
    *heap_top = heap_bottom + HEAP_SIZE;
    alloc_vpages(pgd, heap_bottom, HEAP_SIZE / MEMUNIT_SZ);
}

#define STACK_OFFSET (2 * MEMUNIT_SZ)
#define HEAP_OFFSET  (2 * MEMUNIT_SZ)

PUBLIC program_info load_kmod_from_memory(void *addr) {
    program_info infomations;

    Elf64_Ehdr *header = (Elf64_Ehdr*)addr;

    infomations.entry = header->e_entry;
    infomations.pgd = create_pgd();

    infomations.start = 0xFFFFFFFFFFFFFFFF;
    infomations.end   = 0x0000000000000000;

    for (int i = 0 ; i < header->e_phnum ; i ++) {
        void *start = 0xFFFFFFFFFFFFFFFF;
        void *end   = 0x0000000000000000;

        Elf64_Phdr *pro_header = (Elf64_Phdr*)(addr + header->e_phoff + i * header->e_phentsize);
        load_program (infomations.pgd, addr, pro_header, &start, &end);

        infomations.start = min(infomations.start, start);
        infomations.end   = max(infomations.end  ,   end);
    }

    infomations.stack_top   = (((qword)infomations.start) - STACK_OFFSET + (MEMUNIT_SZ - 1)) & ~(MEMUNIT_SZ - 1);
    infomations.heap_bottom = (((qword)infomations.end)   + HEAP_OFFSET  + (MEMUNIT_SZ - 1)) & ~(MEMUNIT_SZ - 1);

    alloc_stack(infomations.pgd, infomations.stack_top, &infomations.stack_bottom);
    alloc_heap (infomations.pgd, infomations.heap_bottom, &infomations.heap_top);

    mapping_kernel(infomations.pgd);

    return infomations;
}

PUBLIC task_struct *initialize_kmod_task(task_struct *kmod_task) {
    kmod_task->thread_info.basic.rax = KMOD_MAGIC;
    kmod_task->thread_info.basic.rbx = kmod_task->mm_info.heap_start;
    kmod_task->thread_info.basic.rcx = kmod_task->mm_info.heap_end;
    kmod_task->thread_info.basic.rdx = kmod_task->pid;
    return kmod_task;
}