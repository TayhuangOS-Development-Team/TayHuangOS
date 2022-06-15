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
 * syscall.c
 * 
 * 系统调用
 * 
 */

#include "syscall.h"
#include <display/printk.h>
#include <process/task_manager.h>
#include <kheap.h>
#include <debug/logging.h>
#include <memory/paging.h>
#include <string.h>

PRIVATE int __get_pid(void) {
    return current_task->pid; //当前pid
}

#define WAIT_FROM_ANY_MAGIC (-314159)

PRIVATE bool __receive_msg(void *msg, int source) {
    task_struct *src_task = find_task(source);
    if (src_task == NULL) {//目标不存在
        char buffer[128];
        sprintk (buffer, "%d try to receive from %d, but %d doesn't exists!", __get_pid(), source, source);
        lwarn ("Syscall", buffer);
        return false;
    }

    for (msgpack_struct *pack = current_task->ipc_info.queue ; pack != NULL ; pack = pack->next_msg) {
        if (pack->from == source) { //是期望消息
            task_struct *task = find_task(pack->from);
            memcpy(
                __pa(current_task->mm_info->pgd, msg),
                __pa(task->mm_info->pgd, pack->msg),
                pack->len
            ); //拷贝
            if (pack->next_msg != NULL) //出队
                pack->next_msg->last_msg = pack->last_msg;
            if (pack->last_msg != NULL)
                pack->last_msg->next_msg = pack->next_msg;
            if (pack == current_task->ipc_info.queue) { //为队头
                current_task->ipc_info.queue = pack->next_msg;
            }
            task->state = READY;
            kfree(pack);
            return true;
        }
    }
    //没有期望消息
    //等待
    current_task->ipc_info.msg = msg;
    current_task->ipc_info.wait_for = source;
    current_task->state = WAITING_FOR_RECEIVING;
    return true;
}

PRIVATE int __receive_any_msg(void *msg) { //收取第一个消息
    if (current_task->ipc_info.queue == NULL) //无消息
        return -1;

    msgpack_struct *pack = current_task->ipc_info.queue;
    current_task->ipc_info.queue = pack->next_msg;
    if (pack->next_msg != NULL) {
        pack->next_msg->last_msg = NULL; //出队
    }

    task_struct *task = find_task(pack->from);

    int from = pack->from;
    memcpy(
        __pa(current_task->mm_info->pgd ,msg),
        __pa(task->mm_info->pgd, pack->msg),
        pack->len
    );
    task->state = READY;

    kfree(pack); //释放

    return from;
}

PRIVATE int __receive_any_msg_and_wait(void *msg) { //收取第一个消息 如果没有就等待
    if (current_task->ipc_info.queue == NULL) { //无消息
        //没有期望消息
        //等待
        current_task->ipc_info.msg = msg;
        current_task->ipc_info.wait_for = WAIT_FROM_ANY_MAGIC;
        current_task->state = WAITING_FOR_RECEIVING;
        return -1;
    }
    else {
        return __receive_any_msg(msg);
    }
}

PRIVATE bool __send_msg(void *msg, int dest, int len, int tickout) {
    task_struct *dest_task = find_task(dest);
    if (dest_task == NULL) {//目标不存在
        char buffer[128];
        sprintk (buffer, "%d try to sent msg to %d, but %d doesn't exists!", __get_pid(), dest, dest);
        lwarn ("Syscall", buffer);
        return false;
    }
    int pid = __get_pid(); //获取pid
    if (dest_task->ipc_info.wait_for == pid) { //是否在等待该进程来信
        memcpy(
            __pa(dest_task->mm_info->pgd, dest_task->ipc_info.msg),
            __pa(current_task->mm_info->pgd, msg),
            len
        );
        dest_task->ipc_info.wait_for = 0;
        dest_task->ipc_info.msg = NULL;
        dest_task->state = READY; //同步
        return true;
    }
    if (dest_task->ipc_info.wait_for == WAIT_FROM_ANY_MAGIC) { //是否在等待任意进程来信
        memcpy(
            __pa(dest_task->mm_info->pgd, dest_task->ipc_info.msg),
            __pa(current_task->mm_info->pgd, msg),
            len
        );
        dest_task->ipc_info.wait_for = 0;
        dest_task->ipc_info.msg = NULL;
        dest_task->thread_info.rax = pid; //设置返回值
        dest_task->state = READY; //同步
        return true;
    }
    msgpack_struct *pack = (msgpack_struct*)kmalloc(sizeof(msgpack_struct)); //打包
    pack->next_msg = pack->last_msg = NULL;
    pack->from = pid;
    pack->len = len;
    pack->msg = msg;
    if (dest_task->ipc_info.queue == NULL) { //入队
        dest_task->ipc_info.queue = pack;
    }
    else {
        msgpack_struct *queue_tail = dest_task->ipc_info.queue;
        while (queue_tail->next_msg != NULL) queue_tail = queue_tail->next_msg;
        queue_tail->next_msg = pack;
        pack->last_msg = queue_tail;
    }
    current_task->ipc_info.wait_times = tickout;
    current_task->ipc_info.wait_for = dest;
    current_task->state = WAITING_FOR_SENDING; //等待
    return true;
}

extern volatile int ticks;

PRIVATE qword __get_ticks(void) {
    return ticks;
}

PRIVATE qword __sleep(void) {
    current_task->state = WAITING_WAKEUP;
    return true;
}

PRIVATE qword __exit(void) {
    current_task->state = TERMINATE;
    return true;
}

PRIVATE qword __wakeup(int pid) { //sleep的逆操作
    task_struct *task = find_task(pid);
    if (task != NULL) {
        task->state = READY;
        return true;
    }
    return false;
}

PRIVATE qword __fatal(void) {
    current_task->state = EXCEPTION;
    return true;
}

PRIVATE qword __eggs(void) { //彩蛋
    char buffer[64];
    sprintk(buffer, "eggs from pid %d!", __get_pid());
    linfo("Syscall", buffer);
    return 114514;
}

PRIVATE qword __signal(int pid, int signal) {
    task_struct *task = find_task(pid);
    if (task != NULL) {
        task->signal |= (1 << signal);
        return true;
    }
    return false;
}

PRIVATE int wait_pid[16] = {};
PRIVATE bool triggled[16] = {};

PRIVATE qword __wait_irq(int irq) {
    if (! triggled[irq]) {
        __sleep();
        wait_pid[irq] = __get_pid();
    }
    else {
        triggled[irq] = false;
    }
    return true;
}

PUBLIC qword syscall(int sysno, qword mode, qword counter, qword data, void *src, void *dst,
    qword arg1, qword arg2, qword arg3, qword arg4, qword arg5, qword arg6, qword arg7, qword arg8) { //系统调用
    switch (sysno) {
    case 0:
        return __send_msg(src, arg1, data, counter);
    case 1:
        return __receive_msg(dst, arg1);
    case 2:
        return __receive_any_msg(dst);
    case 3:
        return __get_ticks();
    case 4:
        return __sleep();
    case 5:
        return __exit();
    case 6:
        return __wakeup(arg1);
    case 7:
        return __fatal();
    case 8:
        return __eggs();
    case 9:
        return __signal(arg1, data);
    case 10:
        return __wait_irq(data);
    case 11:
        return __receive_any_msg_and_wait(dst);
    }
    return -1;
}

PUBLIC bool send_msg(const void *msg, int dest, int len, int tickout) { //送消息
    return dosyscall(0, 0, tickout, len, (void*)msg, NULL, dest, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC bool receive_msg(const void *msg, int source) { //收特定进程的消息
    return dosyscall(1, 0, 0, 0, NULL, (void*)msg, source, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC int receive_any_msg(const void *msg) { //收消息
    return dosyscall(2, 0, 0, 0, NULL, (void*)msg, 0, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC int get_ticks(void) {
    return dosyscall(3, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC bool sleep(void) {
    return dosyscall(4, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC bool exit(void) {
    bool ret = dosyscall(5, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
    asmv ("ud2"); //不应该执行到此处
    return ret;
}

PUBLIC bool wakeup(int pid) { //sleep的逆操作
    return dosyscall(6, 0, 0, 0, NULL, NULL, pid, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC bool fatal(void) {
    bool ret = dosyscall(7, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
    asmv ("ud2"); //不应该执行到此处
    return ret;
}

PUBLIC int eggs(void) { //彩蛋
    return dosyscall(8, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC bool signal(int pid, int signal) {
    return dosyscall(9, 0, 0, signal, NULL, NULL, pid, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC void wait_irq(int irq) {
    dosyscall(10, 0, 0, irq, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC int receive_any_msg_and_wait(const void *msg) { //收消息
    return dosyscall(11, 0, 0, 0, NULL, (void*)msg, 0, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC bool sendrecv(void *msg, void *ret, int dest, int len, int tickout) {
    if (! send_msg(msg, dest, len, tickout))
        return false;
    if (! receive_msg(ret, dest))
        return false;
    return true;
}

PUBLIC void after_syscall(struct intterup_args *regs);

PUBLIC short wakeup_irq_handler(int irq, struct intterup_args *regs, bool entered_handler) {
    if (wait_pid[irq] != 0) {
        __wakeup(wait_pid[irq]);
    }
    else {
        triggled[irq] = true;
    }
    
    if (! entered_handler)
        after_syscall(regs);

    return 0;
}