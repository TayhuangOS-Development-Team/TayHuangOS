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
 * setup_lm.c
 * 
 * 设置长模式
 * 
 */



#include "setup_lm.h"
#include "load_kernel.h"
#include "lm_operators.h"
#include <printf.h>
#include <int_handlers.h>
#include <tayhuang/paging.h>
#include <tayhuang/control_registers.h>
#include <tayhuang/boot_args.h>
#include <tayhuang/descs.h>
#include <string.h>

PUBLIC void setup_longmode(void *pml4) {
    cr0_t cr0 = get_cr0();
    cr3_t cr3 = get_cr3();
    cr4_t cr4 = get_cr4();
    efer_t efer = get_efer();
    *((int*)&cr3) |= (dword)pml4;
    set_cr3(cr3);
    cr4.pae = true; //物理地址拓展
    set_cr4(cr4);
    efer.lme = true; //Long Mode Enable
    set_efer(efer);
    cr0.pg = true; //启用分页 LMA自动为1
    set_cr0(cr0);
}

PUBLIC void *setup_paging(dword memsz, dword memsz_high, void** limit) {
    const int pte_pmu = (MEMUNIT_SZ / sizeof(PTE)); //PTE Per MEMUNIT
    const int pde_pmu = (MEMUNIT_SZ / sizeof(PDE)); //PDE Per MEMUNIT
    const int pdpte_pmu = (MEMUNIT_SZ / sizeof(PDPTE)); //PDPTE Per MEMUNIT
    const int pml4e_pmu = (MEMUNIT_SZ / sizeof(PML4E)); //PML4E Per MEMUNIT

    const int page_num = memsz / MEMUNIT_SZ
                        + ((memsz % MEMUNIT_SZ) ? 1 : 0) //有余数则 + 1
                        + ((memsz_high) << (32 - sizeof(dword) + leading_zeros(MEMUNIT_SZ))); //memsz_high = mem_size >> 32
                                                                                              //所以(memsz_high << 32) / MEMUNIT_SZ = mem_size << (32 - log2(MEMUNIT_SZ))
                        //最大支持8TB
    const int pte_num = page_num;
    const int pt_num = pte_num / pte_pmu
                        + ((pte_num  % pte_pmu) ? 1 : 0); //有余数则 + 1
    const int pde_num = pt_num;
    const int pd_num = pde_num / pde_pmu
                        + ((pde_num  % pde_pmu) ? 1 : 0); //有余数则 + 1
    const int pdpte_num = pd_num;
    const int pdpt_num = pdpte_num / pdpte_pmu
                        + ((pdpte_num  % pdpte_pmu) ? 1 : 0); //有余数则 + 1
    const int pml4e_num = pdpt_num;
    const int pml4_num = pml4e_num / pml4e_pmu
                        + ((pml4e_num  % pml4e_pmu) ? 1 : 0); //有余数则 + 1
    if (pml4_num != 1) { //4级分页, 支持到8TB
        printf ("Error!Too much memory!\n");
        return NULL;
    }

    PML4E *const pml4_start_address = PAGE_ADDRESS;
    PDPTE *const pdpt_start_address = ((void*)pml4_start_address) + pml4_num * MEMUNIT_SZ;
    PDE *const pd_start_address = ((void*)pdpt_start_address) + pdpt_num * MEMUNIT_SZ;
    PTE *const pt_start_address = ((void*)pd_start_address) + pd_num * MEMUNIT_SZ;
    void *const page_start_address = ((void*)pt_start_address) + pt_num * MEMUNIT_SZ;

    *limit = page_start_address;

    //初始化PT
    for (int i = 0 ; i < pt_num ; i ++) {
        for (int j = 0 ; j < pte_pmu ; j ++) {
            int no = i * pte_pmu + j;
            PTE *pte = pt_start_address + no;
            memset (pte, 0, sizeof(PTE));
            if (no >= pte_num) {
                pte->p = false;
            }
            else {
                pte->p = true;
                pte->rw = true;
                pte->us = false;
                pte->pwt = false;
                pte->pcd = false;
                pte->a = false;
                pte->d = false;
                pte->pat = false;
                pte->g = false;
                pte->xd = false;
                pte->address = no;
            }
        }
    }
    //初始化PD
    for (int i = 0 ; i < pd_num ; i ++) {
        for (int j = 0 ; j < pde_pmu ; j ++) {
            int no = i * pde_pmu + j;
            PDE *pde = pd_start_address + no;
            memset (pde, 0, sizeof(PDE));
            if (no >= pde_num) {
                pde->p = false;
            }
            else {
                pde->p = true;
                pde->rw = true;
                pde->us = false;
                pde->pwt = false;
                pde->pcd = false;
                pde->a = false;
                pde->xd = false;
                pde->address = (((dword)pt_start_address) + no * MEMUNIT_SZ) >> 12;
            }
        }
    }
    //初始化PDPT
    for (int i = 0 ; i < pdpt_num ; i ++) {
        for (int j = 0 ; j < pdpte_pmu ; j ++) {
            int no = i * pdpte_pmu + j;
            PDPTE *pdpte = pdpt_start_address + no;
            memset (pdpte, 0, sizeof(PDPTE));
            if (no >= pdpte_num) {
                pdpte->p = false;
            }
            else {
                pdpte->p = true;
                pdpte->rw = true;
                pdpte->us = false;
                pdpte->pwt = false;
                pdpte->pcd = false;
                pdpte->a = false;
                pdpte->xd = false;
                pdpte->address = (((dword)pd_start_address) + no * MEMUNIT_SZ) >> 12;
            }
        }
    }
    //初始化PML4
    for (int i = 0 ; i < pml4_num ; i ++) {
        for (int j = 0 ; j < pml4e_pmu ; j ++) {
            int no = i * pml4e_pmu + j;
            PML4E *pml4e = pml4_start_address + no;
            memset (pml4e, 0, sizeof(PML4E));
            if (no >= pml4e_num) {
                memset(pml4e, 0, sizeof(PML4E));
            }
            else {
                pml4e->p = true;
                pml4e->rw = true;
                pml4e->us = false;
                pml4e->pwt = false;
                pml4e->pcd = false;
                pml4e->a = false;
                pml4e->xd = false;
                pml4e->address = (((dword)pdpt_start_address) + no * MEMUNIT_SZ) >> 12;
            }
        }
    }
    return pml4_start_address;
}

PRIVATE void init_boot_args(struct boot_args *args, void *setup_mod, void *page_start, void *page_limit,
                            void *kernel_start, void *kernel_limit, bool is_graphic, int screen_width,
                            int screen_height, void *framebuffer, dword memsz, dword memsz_high) {
    memset(args, 0, sizeof(struct boot_args));

    args->magic = BOOT_ARGS_MAGIC; //魔数

    args->is_graphic_mode = is_graphic;
    args->screen_width = screen_width;
    args->screen_height = screen_height;
    args->framebuffer = framebuffer; //视频
    args->framebuffer &= 0xFFFFFFFF;

    args->memory_size = memsz;
    args->memory_size_high = memsz_high; //内存

    args->kernel_start = kernel_start;
    args->kernel_limit = kernel_limit;
    args->page_start = page_start;
    args->page_limit = page_limit; //内存段

    args->setup_mod_addr = setup_mod;
}

PUBLIC void goto_longmode(word selector64, dword memsz, dword memsz_high, bool is_graphic, int screen_width, int screen_height, void *framebuffer) {
    load_result_struct result;
    load_kernel(&result); //加载内核

    if (! result.status) {
        printf ("Error!Couldn't goto long mode!\n");
        return;
    }

    printf ("start: %p, limit: %p, entry: %p", result.kernel_start, result.kernel_limit, result.kernel_entry);

    void *page_limit;
    void *page_start = setup_paging(memsz, memsz_high, &page_limit); //设置分页

    if (page_start == NULL) {
        printf ("Error!Could setup paging!\n");
        return;
    }

    static struct boot_args _args;
    init_boot_args(&_args, result.setup_mod, page_start, page_limit, result.kernel_start, result.kernel_limit,
                   is_graphic, screen_width, screen_height, framebuffer, memsz, memsz_high); //初始化引导参数

    for (int i = 0 ; i < 16 ; i ++)
        disable_irq(i); //禁用所有IRQ

    asmv ("cli"); //关中断

    setup_longmode(page_start); //设置长模式

    static struct desc_ptr _idtr = {.address = 0, .size = 0};
    asmv ("lidt %0" : : "m"(_idtr)); //重置IDTR

    goto_ia32e(result.kernel_entry, &_args, selector64); //进入IA32-e模式
}