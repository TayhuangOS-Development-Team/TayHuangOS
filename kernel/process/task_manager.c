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
 * task_manager.c
 *
 * 进程管理器
 *
 */



#include "task_manager.h"

#include <tayhuang/io.h>
#include <tayhuang/control_registers.h>

#include <kheap.h>
#include <memory/paging.h>
#include <memory/pmm.h>

PRIVATE mm_struct *create_mm_struct(void *pgd, qword start_code, qword end_code, qword start_data, qword end_data, qword start_brk, qword end_brk,
    qword start_rodata, qword end_rodata, qword start_stack, qword end_stack) { //创建MM Info
    mm_struct *mm = malloc(sizeof(mm_struct));

    mm->areas = create_vmarea(); //虚空间树根
    mm->pgd = pgd; //页表

    mm->start_code   = start_code;
    mm->end_code     = end_code;
    mm->start_data   = start_data;
    mm->end_data     = end_data;
    mm->start_brk    = start_brk;
    mm->end_brk      = end_brk;
    mm->start_rodata = start_rodata;
    mm->end_rodata   = end_rodata;
    mm->start_stack  = start_stack;
    mm->end_stack  = end_stack;

    return mm;
}

PRIVATE task_struct *create_task_struct(int pid, int priority, void *entry, qword rflags, word cs, void *pgd, qword start_code, qword end_code,
    qword start_data, qword end_data, qword start_brk, qword end_brk, qword start_rodata, qword end_rodata, qword start_stack, qword end_stack) { //创建PCB
    task_struct *task = malloc(sizeof(task_struct));
    memset(&task->thread_info, 0, sizeof(task->thread_info));
    task->thread_info.rip = entry; //入口
    task->thread_info.rflags = rflags;
    task->thread_info.cs = cs; //代码段
    task->thread_info.ss = cs - 8; //数据段在代码段上一个
    task->thread_info.ds = cs - 8;

    task->counter = 0;
    task->priority = priority;

    task->state = SUBBMITED; //提交态
    task->next = task->last = NULL;
    task->pid = pid; //PID
    memset (&task->signal, 0, sizeof (task->signal));
    task->mm_info = create_mm_struct(pgd, start_code, end_code, start_data, end_data,
        start_brk, end_brk, start_rodata, end_rodata, start_stack, end_stack); //创建MM

    task->ipc_info.msg = NULL; //初始化IPC信息
    task->ipc_info.queue = NULL;
    task->ipc_info.wait_for = 0;
    return task;
}

PUBLIC task_struct *task_table; //进程表
PRIVATE int __cur_pid = 0;

PRIVATE int alloc_pid(void) { //自动分配PID
    return __cur_pid ++;
}

PUBLIC task_struct *create_task(int priority, void *entry, qword rflags, qword rsp, qword rbp, word cs, void *pgd, qword start_pos, qword end_pos) { //创建进程
    dis_int();
    task_struct *task = create_task_struct(alloc_pid(), priority, entry, rflags, cs, pgd,
         start_pos, end_pos, start_pos, end_pos, start_pos, end_pos, start_pos, end_pos, rsp, rbp);
    task->next = task_table;
    if (task_table != NULL)
        task_table->last = task;
    task_table = task;
    task->thread_info.rsp = rsp;
    en_int();
    return task;
}

PRIVATE void do_mem_copy(void *start_addr, void *end_addr, void *pml4) { //复制
    set_pml4(pml4);
    while (start_addr < end_addr) {
        int page_need = (end_addr - start_addr) / 4096;
        int page_num = 0;
        void *free_page = find_freepages(min(page_need, 64), &page_num);

        for (int i = 0 ; i < page_num ; i ++)
            mark_used(free_page + i * 4096);

        memcpy(start_addr, free_page, MEMUNIT_SZ * page_num);
        
        set_mapping(start_addr, free_page, page_num, true, true);
        start_addr += page_num * 4096;
    }
}

PUBLIC task_struct *do_fork_execv(int priority, void(*entry)(void), qword rsp, qword rbp) {
    //TODO
    //WIP
    dis_int();
    task_struct *task = create_task_struct(alloc_pid(), priority, entry,
         current_task->thread_info.rsp, current_task->thread_info.cs, current_task->mm_info->pgd,
         current_task->mm_info->start_code, current_task->mm_info->end_code,
         current_task->mm_info->start_data, current_task->mm_info->end_data,
         current_task->mm_info->start_brk, current_task->mm_info->end_brk,
         current_task->mm_info->start_rodata, current_task->mm_info->end_rodata,
         rsp, rbp);
    task->next = task_table;
    if (task_table != NULL)
        task_table->last = task;
    task_table = task;
    task->thread_info.rsp = rsp;

    void *pml4 = create_pgd(); //页表
    set_pml4(pml4);
    set_mapping(0, 0, 16384, true, true);
    do_mem_copy(current_task->mm_info->start_code,   current_task->mm_info->end_code,   pml4);
    do_mem_copy(current_task->mm_info->start_data,   current_task->mm_info->end_data,   pml4);
    do_mem_copy(current_task->mm_info->start_brk,    current_task->mm_info->end_brk,    pml4);
    do_mem_copy(current_task->mm_info->start_rodata, current_task->mm_info->end_rodata, pml4);
    do_mem_copy(rsp,                                 rbp,                               pml4);
    task->mm_info->pgd = pml4;
    en_int();

    return task;
}

PUBLIC task_struct *current_task = NULL;

PUBLIC void do_switch(struct intterup_args *regs) { //进行进程切换
    //下降进程
    current_task->thread_info.rsp = regs->rsp;
    current_task->thread_info.rip = regs->rip;
    current_task->thread_info.cs = regs->cs;
    current_task->thread_info.ds = regs->ds;
    current_task->thread_info.es = regs->es;
    current_task->thread_info.fs = regs->fs;
    current_task->thread_info.gs = regs->gs;
    current_task->thread_info.ss = regs->ss;
    current_task->thread_info.rflags = regs->rflags;
    current_task->mm_info->pgd = (void*)(regs->pgd & 0xFFFFFFFFFFFFF000);
    //common registers
    current_task->thread_info.rax = regs->rax;
    current_task->thread_info.rbx = regs->rbx;
    current_task->thread_info.rcx = regs->rcx;
    current_task->thread_info.rdx = regs->rdx;
    current_task->thread_info.rsi = regs->rsi;
    current_task->thread_info.rdi = regs->rdi;
    current_task->thread_info.rbp = regs->rbp;

    current_task->thread_info.r8 = regs->r8;
    current_task->thread_info.r9 = regs->r9;
    current_task->thread_info.r10 = regs->r10;
    current_task->thread_info.r11 = regs->r11;
    current_task->thread_info.r12 = regs->r12;
    current_task->thread_info.r13 = regs->r13;
    current_task->thread_info.r14 = regs->r14;
    current_task->thread_info.r15 = regs->r15;
    if (current_task->state == RUNNING) //若是因时间片结束
        current_task->state = READY;

    //switch
    task_struct *nxt_task = current_task->next ? current_task->next : task_table;
    do {
        current_task = nxt_task;
        if (current_task->state == WAITING_FOR_SENDING) {
            if (current_task->ipc_info.wait_times > 0) {
                current_task->ipc_info.wait_times --;
            }
            else if (current_task->ipc_info.wait_times != -1) {
                for (task_struct *cur = task_table ; cur != NULL ; cur = cur->next) { //遍历task表
                    if (cur->pid == current_task->ipc_info.wait_for) {
                        for (msgpack_struct *pack = cur->ipc_info.queue ; pack != NULL ; pack = pack->next_msg) {
                            if (pack->from == current_task->pid) {
                                if (pack->next_msg != NULL) //出队
                                    pack->next_msg->last_msg = pack->last_msg;
                                if (pack->last_msg != NULL)
                                    pack->last_msg->next_msg = pack->next_msg;
                                free(pack);
                                break;
                            }
                        }
                        break;
                    }
                }
                current_task->thread_info.rax = 0;
                current_task->ipc_info.wait_for = 0;
                current_task->state = READY; //可以运行
            }
        }
        else if (current_task->state == TERMINATE || current_task->state == EXCEPTION) { //回收
            if (current_task->next) //删除节点
                current_task->next->last = current_task->last;
            if (current_task->last)
                current_task->last->next = current_task->next;

            for (msgpack_struct *nxt, *pack = current_task->ipc_info.queue ; pack != NULL ; pack = nxt) { //清除IPC
                nxt = pack->next_msg;
                free(pack);
            }
            nxt_task = current_task->next ? current_task->next : task_table;
            free(current_task); //释放

            continue;
        }
        //set counter
        current_task->counter = ((current_task->counter >> 1) + current_task->priority);

        nxt_task = current_task->next ? current_task->next : task_table;
    } while (current_task->state != READY && current_task->state != SUBBMITED); //直到它为符合条件的进程

    //上升进程
    regs->rsp = current_task->thread_info.rsp;
    regs->rip = current_task->thread_info.rip;
    regs->cs = current_task->thread_info.cs;
    regs->ss = current_task->thread_info.ss;
    regs->ds = current_task->thread_info.ds;
    regs->es = current_task->thread_info.es;
    regs->fs = current_task->thread_info.fs;
    regs->gs = current_task->thread_info.gs;
    regs->rflags = current_task->thread_info.rflags;
    regs->pgd &= 0xFFF;
    regs->pgd |= (((qword)current_task->mm_info->pgd) & 0xFFFFFFFFFFFFF000);
    //common registers
    regs->rax = current_task->thread_info.rax;
    regs->rbx = current_task->thread_info.rbx;
    regs->rcx = current_task->thread_info.rcx;
    regs->rdx = current_task->thread_info.rdx;
    regs->rsi = current_task->thread_info.rsi;
    regs->rdi = current_task->thread_info.rdi;
    regs->rbp = current_task->thread_info.rbp;

    regs->r8 = current_task->thread_info.r8;
    regs->r9 = current_task->thread_info.r9;
    regs->r10 = current_task->thread_info.r10;
    regs->r11 = current_task->thread_info.r11;
    regs->r12 = current_task->thread_info.r12;
    regs->r13 = current_task->thread_info.r13;
    regs->r14 = current_task->thread_info.r14;
    regs->r15 = current_task->thread_info.r15;
    current_task->state = RUNNING; //设置状态
}