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
#include <task/task_manager.h>

#include <logging.h>

#include <memory/pmm.h>
#include <memory/paging.h>

#include <string.h>

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

PUBLIC bool __send_msg(void *src, qword size, int dst) {
    task_struct *target = get_task_by_pid(dst);

    if ((target == NULL) || 
        ((target->ipc_info.used_size + size + sizeof(msgpack_struct)) > target->ipc_info.mail_size) ||
        ((target->ipc_info.allow_pid != current_task->pid) && (target->ipc_info.allow_pid >= 0)))
    {
        return false;
    }

    void *addr = target->ipc_info.write_ptr;
    msgpack_struct *pack = (msgpack_struct*)addr;

    target->ipc_info.write_ptr = addr + sizeof(msgpack_struct) + size;
    pack->length = size;
    pack->source = current_task->pid;

    target->ipc_info.used_size += sizeof(msgpack_struct) + size;

    vvmemcpy(target->mm_info.pgd, pack->body, current_task->mm_info.pgd, src, size);

    if (target->state == WAITING && target->ipc_info.allow_pid != DUMMY_TASK) {
        target->state = READY;

        if (target->level == 0) {
            enqueue_level0_task(target);
        }
        else {
            enqueue_level1_task(target);
        }
    }
    return true;
}

PUBLIC bool send_msg(void *src, qword size, int dst) {
    return dosyscall(SEND_MSG_SN, 0, size, dst, src, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

//-------------------

PUBLIC void __check_ipc(void) {
    if (current_task->ipc_info.allow_pid != DUMMY_TASK) {
        if (current_task->ipc_info.used_size > 0) {
            return;
        }
    }

    current_task->state = WAITING;
}

PUBLIC void check_ipc(void) {
    dosyscall(CHECK_IPC_SN, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

//-------------------

PUBLIC void __set_allow(int pid) {
    current_task->ipc_info.allow_pid = pid;
}

PUBLIC void set_allow(int pid) {
    dosyscall(SET_ALLOW_SN, 0, 0, pid, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

//------------------

PUBLIC int __recv_msg(void *dst) {
    if (current_task->ipc_info.used_size <= 0) {
        return -1;
    }

    void *addr = current_task->ipc_info.read_ptr;
    msgpack_struct *pack = (msgpack_struct*)addr;

    current_task->ipc_info.read_ptr = addr + sizeof(msgpack_struct) + pack->length;
    
    current_task->ipc_info.used_size -= (sizeof(msgpack_struct) + pack->length);

    vvmemcpy(current_task->mm_info.pgd, dst, current_task->mm_info.pgd, pack->body, pack->length);

    return pack->source;
}

PUBLIC int recv_msg(void *dst) {
    return dosyscall(RECV_MSG_SN, 0, 0, 0, NULL, dst, 0, 0, 0, 0, 0, 0, 0, 0);
}

//-------------------

PUBLIC void __set_mailbuffer(void *buffer, qword size) {
    //设置指针
    current_task->ipc_info.read_ptr = current_task->ipc_info.write_ptr = current_task->ipc_info.mail = buffer;
    //设置邮箱大小
    current_task->ipc_info.mail_size = size;
    //设置已使用大小
    current_task->ipc_info.used_size = 0;
}

PUBLIC void set_mailbuffer(void *buffer, qword size) {
    dosyscall(SET_MAILBUFFER_SN, 0, size, 0, NULL, buffer, 0, 0, 0, 0, 0, 0, 0, 0);
}

//--------------------

PUBLIC bool dummy_send_msg(void *src, qword size, int dst) {
    task_struct *target = get_task_by_pid(dst);

    if ((target == NULL) || 
        ((target->ipc_info.used_size + size + sizeof(msgpack_struct)) > target->ipc_info.mail_size) ||
        ((target->ipc_info.allow_pid != current_task->pid) && (target->ipc_info.allow_pid >= 0)))
    {
        return false;
    }

    void *addr = target->ipc_info.write_ptr;
    msgpack_struct *pack = (msgpack_struct*)addr;

    target->ipc_info.write_ptr = addr + sizeof(msgpack_struct) + size;
    pack->length = size;
    pack->source = DUMMY_TASK;

    target->ipc_info.used_size += sizeof(msgpack_struct) + size;

    pvmemcpy(target->mm_info.pgd, pack->body, src, size);

    if (target->state == WAITING) {
        target->state = READY;

        if (target->level == 0) {
            enqueue_level0_task(target);
        }
        else {
            enqueue_level1_task(target);
        }
    }
    return true;
}