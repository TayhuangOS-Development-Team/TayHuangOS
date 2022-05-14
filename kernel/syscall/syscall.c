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
#include "../display/printk.h"
#include "../process/task_manager.h"
#include "../kheap.h"

PRIVATE int __get_pid(void) {
    return current_task->pid; //当前pid
}

PRIVATE task_struct *__find_task(int pid) {
    for (task_struct *cur = task_table ; cur != NULL ; cur = cur->next) { //遍历task表
        if (cur->pid == pid)
            return cur;
    }
    return NULL;
}

PRIVATE bool __receive_msg(void *msg, int source) {
    task_struct *source_task = __find_task(source); //获取pid所对的task
    if (source_task == NULL) { //不存在
        return false;
    }
    int cur_pid = __get_pid();
    if (source_task->ipc_info.recv_send && source_task->ipc_info.wait_for == cur_pid) { //对方已经发送
        memcpy(msg, source_task->ipc_info.msg, source_task->ipc_info.len);
        memset(&source_task->ipc_info, 0, sizeof(ipc_struct));
        source_task->state = READY;
    }
    else { //对方未发送
        //死锁处理以后再做
        current_task->ipc_info.msg = msg;
        current_task->ipc_info.len = -1;
        current_task->ipc_info.recv_send = false;
        current_task->ipc_info.wait_for = source;
        current_task->state = WAITING_FOR_RECEIVING; //等待对方发送
    }
    return true;
}

PRIVATE bool __send_msg(void *msg, int dest, int len) {
    task_struct *dest_task = __find_task(dest); //获取pid所对的task
    if (dest_task == NULL) { //不存在
        return false;
    }
    int cur_pid = __get_pid();
    if ((! dest_task->ipc_info.recv_send) && dest_task->ipc_info.wait_for == cur_pid) { //对方已经等待接收
        memcpy(dest_task->ipc_info.msg, msg, len);
        memset(&dest_task->ipc_info, 0, sizeof(ipc_struct));
        dest_task->state = READY;
    }
    else { //对方未发送
        //死锁处理以后再做
        current_task->ipc_info.msg = msg;
        current_task->ipc_info.len = len;
        current_task->ipc_info.recv_send = true;
        current_task->ipc_info.wait_for = dest;
        current_task->state = WAITING_FOR_SENDING; //等待对方接收
    }
    return true;
}

PUBLIC qword syscall(int sysno, qword mode, qword counter, qword data, void *src, void *dst,
    qword arg1, qword arg2, qword arg3, qword arg4, qword arg5, qword arg6, qword arg7, qword arg8) { //系统调用
    if (sysno == 0x00) {
        return __send_msg(src, arg1, counter);
    }
    if (sysno == 0x01) {
        return __receive_msg(dst, arg1);
    }
    return -1;
}