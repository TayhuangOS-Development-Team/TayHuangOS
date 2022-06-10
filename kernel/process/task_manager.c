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
#include <memory/shared_memory.h>

#include <debug/logging.h>
#include <syscall/syscall.h>

#include <display/printk.h>

#include <string.h>

PRIVATE mm_struct *create_mm_struct(void *pgd, qword start_code, qword end_code, qword start_data, qword end_data, qword start_heap, qword end_heap,
    qword start_rodata, qword end_rodata, qword start_stack, qword end_stack) { //创建MM Info
    mm_struct *mm = kmalloc(sizeof(mm_struct));

    mm->areas = create_vmarea(); //虚空间树根
    mm->pgd = pgd; //页表

    mm->start_code   = start_code;
    mm->end_code     = end_code;
    mm->start_data   = start_data;
    mm->end_data     = end_data;
    mm->start_heap    = start_heap;
    mm->end_heap      = end_heap;
    mm->start_rodata = start_rodata;
    mm->end_rodata   = end_rodata;
    mm->start_stack  = start_stack;
    mm->end_stack  = end_stack;

    return mm;
}

PRIVATE task_struct *create_task_struct(int pid, int priority, void *entry, qword rflags, word cs, void *pgd, qword start_code, qword end_code,
    qword start_data, qword end_data, qword start_heap, qword end_heap, qword start_rodata, qword end_rodata, qword start_stack, qword end_stack) { //创建PCB

    task_struct *task = kmalloc(sizeof(task_struct));
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
        start_heap, end_heap, start_rodata, end_rodata, start_stack, end_stack); //创建MM

    task->ipc_info.msg = NULL; //初始化IPC信息
    task->ipc_info.queue = NULL;
    task->ipc_info.wait_for = 0;
    return task;
}

PUBLIC task_struct *task_table; //进程表

PUBLIC task_struct *create_task(int pid, int priority, void *entry, qword rflags, qword rsp, qword rbp, word cs, void *pgd,
    qword start_pos, qword end_pos, qword start_heap, qword end_heap) {//创建进程

    dis_int();
    task_struct *task = create_task_struct(pid, priority, entry, rflags, cs, pgd,
         start_pos, end_pos, start_pos, end_pos, start_heap, end_heap, start_pos, end_pos, rbp, rsp);
    if (priority != -1) {
        task->next = task_table;
        if (task_table != NULL)
            task_table->last = task;
        task_table = task;
        task->thread_info.rsp = rsp;
    }
    en_int();
    return task;
}

PUBLIC task_struct *current_task = NULL;
PUBLIC task_struct *empty_task = NULL;

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
    task_struct *cur_task = current_task->next;
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
                                kfree(pack);
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
                kfree(pack);
            }
            nxt_task = current_task->next ? current_task->next : task_table;
            kfree(current_task); //释放

            continue;
        }
        //set counter
        current_task->counter = ((current_task->counter >> 1) + current_task->priority);

        nxt_task = current_task->next ? current_task->next : task_table;
        if (nxt_task == cur_task) { //实在没进程运行了
            empty_task->next = current_task;
            current_task = empty_task;
        }
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

PUBLIC task_struct *find_task(int pid) {
    for (task_struct *cur = task_table ; cur != NULL ; cur = cur->next) { //遍历task表
        if (cur->pid == pid)
            return cur;
    }
    return NULL;
}

#define GET_CURRENT_PID (0)
#define GET_START_HEAP (1)
#define GET_END_HEAP (2)
#define GET_TASK_START (3)
#define GET_TASK_END (4)
#define SHM_MAPPING (5)
#define GET_TASK_PGD (6)
#define SET_TASK_PGD (7)
#define LOGGING_ALL_TASK (8)
#define GET_TASK_START_WITHOUT_STACK (9)
#define GET_TASK_END_WITHOUT_HEAP (10)
#define SET_MAPPING_FROM (11)

PRIVATE const char *state_to_string(int state) {
    switch (state)
    {
    case SUBBMITED: return "submmited";
    case READY: return "ready";
    case WAITING_WAKEUP: return "waiting wakeup";
    case WAITING_FOR_SENDING: return "waiting for sending";
    case WAITING_FOR_RECEIVING: return "waiting for receiving";
    case RUNNING: return "running";
    case EXCEPTION: return "exception";
    case TERMINATE: return "terminate";
    default: return "unknown";
    }
}

PRIVATE void log_task(task_struct *task) {
    char buffer[512];
    sprintk (
        buffer,
        "\n\
------------------------------------------------------\n\
Task(PID=%d) counter=%d priority=%d state=%s\n\
PGD=%P\nCode=%P~%P\nData=%P~%P\nStack=%P~%P\nRodata=%P~%P\nHeap=%P~%P\n\
------------------------------------------------------\
",
        task->pid, task->counter, task->priority, state_to_string(task->state),
        task->mm_info->pgd,
        task->mm_info->start_code, task->mm_info->end_code,
        task->mm_info->start_data, task->mm_info->end_data,
        task->mm_info->start_stack, task->mm_info->end_stack,
        task->mm_info->start_rodata, task->mm_info->end_rodata,
        task->mm_info->start_heap, task->mm_info->end_heap
    );
    linfo ("Taskman", buffer);
}

//task manager进程
PUBLIC void taskman(void) {
    char buffer[128];
    while (true) {
        qword pack[20];
        int caller = 0;
        while ((caller = receive_any_msg(pack)) == -1);
        int cmdid = pack[0];
        switch (cmdid)
        {
        case GET_CURRENT_PID: {
            int current_pid = caller;
            send_msg(&current_pid, caller, sizeof(current_pid), 20);
            break;
        }
        case GET_START_HEAP: {
            void *start_heap = find_task(pack[1])->mm_info->start_heap;
            send_msg(&start_heap, caller, sizeof(start_heap), 20);
            break;
        }
        case GET_END_HEAP: {
            void *end_heap = find_task(pack[1])->mm_info->end_heap;
            send_msg(&end_heap, caller, sizeof(end_heap), 20);
            break;
        }
        case GET_TASK_START: {
            void *start = find_task(pack[1])->mm_info->start_stack;
            send_msg(&start, caller, sizeof(start), 20);
            break;
        }
        case GET_TASK_END: {
            void *end = find_task(pack[1])->mm_info->end_heap;
            send_msg(&end, caller, sizeof(end), 20);
            break;
        }
        case SHM_MAPPING: {
            void *addr = pack[1];
            int pages = pack[2];
            int src_pid = caller;
            int target_pid = pack[3];
            bool status = shm_mapping(addr, pages, src_pid, target_pid);
            send_msg (&status, caller, sizeof(status), 20);
            break;
        }
        case GET_TASK_PGD: {
            void *pgd = find_task(pack[1])->mm_info->pgd;
            send_msg(&pgd, caller, sizeof(pgd), 20);
            break;
        }
        case SET_TASK_PGD: {
            find_task(pack[1])->mm_info->pgd = pack[2];
            bool status = true;
            send_msg (&status, caller, sizeof(status), 20);
            break;
        }
        case LOGGING_ALL_TASK: {
            for (task_struct *task = task_table ; task != NULL ; task = task->next) {
                log_task(task);
            }
            break;
        }
        case GET_TASK_START_WITHOUT_STACK: {
            qword start = find_task(pack[1])->mm_info->start_code;
            start = min(start, find_task(pack[1])->mm_info->start_data);
            start = min(start, find_task(pack[1])->mm_info->start_rodata);
            send_msg(&start, caller, sizeof(start), 20);
            break;
        }
        case GET_TASK_END_WITHOUT_HEAP: {
            qword end = find_task(pack[1])->mm_info->end_code;
            end = max(end, find_task(pack[1])->mm_info->end_data);
            end = max(end, find_task(pack[1])->mm_info->end_rodata);
            send_msg(&end, caller, sizeof(end), 20);
            break;
        }
        case SET_MAPPING_FROM: {
            void *src_addr = pack[1];
            void *dst_addr = pack[2];
            int pages = pack[3];
            int src_pid = pack[4];
            set_pml4(find_task(caller)->mm_info->pgd);
            for (int i = 0 ; i < pages ; i ++)
                set_mapping(dst_addr + i * MEMUNIT_SZ, __pa(find_task(src_pid)->mm_info->pgd, src_addr + i * MEMUNIT_SZ), 1, true, true);
            bool status = true;
            send_msg (&status, caller, sizeof(status), 20);
            break;
        }
        default: {
            sprintk (buffer, "TASKMAN received an unknown command %d from %d", cmdid, caller);
            lerror ("Taskman", buffer);
            break;
        }
        }
    }
}