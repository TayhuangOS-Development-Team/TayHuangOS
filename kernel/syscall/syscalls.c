/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong
 * 
 * syscalls.c
 * 
 * 系统调用的实现
 * 
 */



#include <syscall/syscalls.h>
#include <syscall/syscall.h>

#include <task/task_scheduler.h>

#include <logging.h>

PUBLIC void __moo(void) {
    linfo ("COW", "                       (__)");
    linfo ("COW", "                       (oo)");
    linfo ("COW", "            /-----------\\/");
    linfo ("COW", "           / |  MOO     ||");
    linfo ("COW", "          /  |    COW   ||");
    linfo ("COW", "         *  /\\----------/\\");
    linfo ("COW", "            ~~          ~~");
    linfo ("COW", "Moo~Have you mooed today?Moo with me, moo~");
}

PUBLIC void moo(void) {
    dosyscall(MOO_SN, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

//-------------------

PUBLIC void __setmail_buffer(void *buffer, qword size) {
    current_task->ipc_info.current_ptr = current_task->ipc_info.mail = buffer;
    current_task->ipc_info.mail_size = size;
}

PUBLIC void setmail_buffer(void *buffer, qword size) {
    dosyscall(SETMAIL_BUFFER_SN, 0, size, 0, NULL, buffer, 0, 0, 0, 0, 0, 0, 0, 0);
}

//-------------------

PUBLIC void __wait_ipc(int pid) {
    current_task->state = WAITING_IPC;
    current_task->wait_pid = pid;
}

PUBLIC void wait_ipc(int pid) {
    dosyscall(WAIT_IPC_SN, 0, 0, pid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}