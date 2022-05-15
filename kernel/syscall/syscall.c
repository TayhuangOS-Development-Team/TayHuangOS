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
    for (msgpack_struct *pack = current_task->ipc_info.queue ; pack != NULL ; pack = pack->next_msg) {
        if (pack->from == source) { //是期望消息
            memcpy(msg, pack->msg, pack->len); //拷贝
            if (pack->next_msg != NULL) //出队
                pack->next_msg->last_msg = pack->last_msg;
            if (pack->last_msg != NULL)
                pack->last_msg->next_msg = pack->next_msg;
            __find_task(pack->from)->state = READY;
            free(pack);
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

PUBLIC int __receive_any_msg(void *msg) { //收取第一个消息
    if (current_task->ipc_info.queue == NULL) //无消息
        return -1;

    msgpack_struct *pack = current_task->ipc_info.queue;
    current_task->ipc_info.queue = pack->next_msg;
    if (pack->next_msg != NULL) {
        pack->next_msg->last_msg = NULL; //出队
    }

    int from = pack->from;
    memcpy(msg, pack->msg, pack->len);
    __find_task(pack->from)->state = READY;

    free(pack); //释放

    return from;
}

PRIVATE bool __send_msg(void *msg, int dest, int len) {
    task_struct *dest_task = __find_task(dest);
    if (dest_task == NULL) //目标不存在
        return false;
    int pid = __get_pid(); //获取pid
    if (dest_task->ipc_info.wait_for == pid) { //是否在等待该进程来信
        memcpy(dest_task->ipc_info.msg, msg, len);
        dest_task->ipc_info.wait_for = 0;
        dest_task->ipc_info.msg = NULL;
        dest_task->state = READY; //同步
        return true;
    }
    msgpack_struct *pack = (msgpack_struct*)malloc(sizeof(msgpack_struct)); //打包
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
    current_task->state = WAITING_FOR_SENDING; //等待
    return true;
}

PUBLIC qword syscall(int sysno, qword mode, qword counter, qword data, void *src, void *dst,
    qword arg1, qword arg2, qword arg3, qword arg4, qword arg5, qword arg6, qword arg7, qword arg8) { //系统调用
    if (sysno == 0) {
        return __send_msg(src, arg1, counter);
    }
    else if (sysno == 1) {
        return __receive_msg(dst, arg1);
    }
    else if (sysno == 2) {
        return __receive_any_msg(dst);
    }
    return -1;
}

bool send_msg(void *msg, int dest, int len) { //送消息
    return dosyscall(0, 0, len, 0, msg, NULL, dest, 0, 0, 0, 0, 0, 0, 0);
}

bool receive_msg(void *msg, int source) { //收特定进程的消息
    return dosyscall(1, 0, 0, 0, NULL, msg, source, 0, 0, 0, 0, 0, 0, 0);
}

int receive_any_msg(void *msg) { //收消息
    return dosyscall(2, 0, 0, 0, NULL, msg, 0, 0, 0, 0, 0, 0, 0, 0);
}