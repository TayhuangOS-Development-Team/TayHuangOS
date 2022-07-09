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

PUBLIC bool __sendmsg(void *src, qword size, int dst) {
    task_struct *target = get_task_by_pid(dst);

    //剩余空间不足
    if (target->ipc_info.used_size + sizeof(msgpack_struct) + size > target->ipc_info.mail_size) {
        return false;
    }

    //获取消息包地址
    msgpack_struct *pack = (msgpack_struct*)target->ipc_info.write_ptr;
    //更新写入指针
    target->ipc_info.write_ptr += sizeof(msgpack_struct) + size; 

    //更新已使用大小
    target->ipc_info.used_size += sizeof(msgpack_struct) + size; 

    //写入指针超出队列
    if (target->ipc_info.write_ptr >= (target->ipc_info.mail + target->ipc_info.mail_size)) { 
        //剩余空间不足
        if (target->ipc_info.mail + sizeof(msgpack_struct) + size > target->ipc_info.read_ptr) { 
            //还原
            target->ipc_info.write_ptr -= sizeof(msgpack_struct) + size; 
            target->ipc_info.used_size -= sizeof(msgpack_struct) + size;
            return false;
        }

        //设置新地址
        pack = target->ipc_info.mail; 
        //设置新写入指针
        target->ipc_info.write_ptr = target->ipc_info.mail + sizeof(msgpack_struct) + size; 
        //更新已使用大小
        target->ipc_info.used_size = target->ipc_info.mail_size - (target->ipc_info.read_ptr - target->ipc_info.write_ptr); 
    }

    //临时变量(不能直接操作pack(为虚拟地址))
    msgpack_struct _pack; 

    _pack.source = current_task->pid;
    _pack.length = size;
    _pack.next = NULL;

    //复制到缓冲区
    pvmemcpy(target->mm_info.pgd, pack, &_pack, sizeof(msgpack_struct)); 

    //有上一条消息
    if (target->ipc_info.lastest_msg != NULL) {
        //从缓冲区复制
        vpmemcpy(&_pack, target->mm_info.pgd, target->ipc_info.lastest_msg, sizeof(msgpack_struct)); 
        _pack.next = pack;
        //复制到缓冲区
        pvmemcpy(target->mm_info.pgd, target->ipc_info.lastest_msg, &_pack, sizeof(msgpack_struct)); 
    }
    
    //复制正文
    vvmemcpy(target->mm_info.pgd, pack->body, current_task->mm_info.pgd, src, size);

    if ((target->wait_pid == ANY_TASK) || (target->wait_pid == current_task->pid)) {
        //唤醒目标
        target->wait_pid = NULL_TASK;
        target->state = READY;

        if (target->level == 0) {
            enqueue_level0_task(target);
        }
        else if (target->level == 1) {
            enqueue_level1_task(target);
        }
    }

    return true;
}

PUBLIC bool sendmsg(void *src, qword size, int dst) {
    return dosyscall(SENDMSG_SN, 0, size, dst, src, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

//-------------------

PUBLIC void __wait_ipc(int pid) {
    //是等待任意进程
    if (pid == ANY_TASK) {
        //mail不为空
        if (__get_used_size() != 0) {
            return;
        }
    }
    else {
        //mail不为空
        if (__get_used_size() != 0) {
            msgpack_struct pack;
            for (msgpack_struct *read_ptr = __get_read_ptr() ; read_ptr != NULL ; ) {
                vpmemcpy(&pack, current_task->mm_info.pgd, read_ptr, sizeof(msgpack_struct));
                //是等待的进程
                if (pack.source == pid) {
                    return;
                }
                read_ptr = pack.next;
            }
        }
    }

    current_task->state = WAITING_IPC;
    current_task->wait_pid = pid;
}

PUBLIC void wait_ipc(int pid) {
    dosyscall(WAIT_IPC_SN, 0, 0, pid, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

//-------------------

PUBLIC void *__get_read_ptr(void) {
    return current_task->ipc_info.read_ptr;
}

PUBLIC void *get_read_ptr(void) {
    return (void*)dosyscall(GET_READ_PTR_SN, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

//-------------------

PUBLIC void __set_read_ptr(void *read_ptr) {
    current_task->ipc_info.read_ptr = read_ptr;
}

PUBLIC void set_read_ptr(void *read_ptr){
    dosyscall(GET_READ_PTR_SN, 0, 0, 0, read_ptr, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

//-------------------

PUBLIC qword __get_used_size(void) {
    return current_task->ipc_info.used_size;
}

PUBLIC qword get_used_size(void) {
    return dosyscall(GET_USED_SIZE_SN, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

//-------------------

PUBLIC void __set_used_size(qword used_size) {
    current_task->ipc_info.used_size = used_size;
}

PUBLIC void set_used_size(qword used_size) {
    dosyscall(SET_USED_SIZE_SN, 0, used_size, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

//-------------------

PUBLIC void __setmail_buffer(void *buffer, qword size) {
    //设置指针
    current_task->ipc_info.read_ptr = current_task->ipc_info.write_ptr = current_task->ipc_info.mail = buffer;
    //设置邮箱大小
    current_task->ipc_info.mail_size = size;
    //设置已使用大小
    current_task->ipc_info.used_size = 0;
}

PUBLIC void setmail_buffer(void *buffer, qword size) {
    dosyscall(SETMAIL_BUFFER_SN, 0, size, 0, NULL, buffer, 0, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC bool recvmsg(void *dst, int src) {
    msgpack_struct *last = NULL;
    for (msgpack_struct *pack = __get_read_ptr() ; pack != NULL ; last = pack, pack = pack->next) {
        //是等待的进程
        if (pack->source == src) {
            if (last != NULL) {
                last->next = pack->next;
                set_read_ptr(pack->next == NULL ? (((void*)pack) + pack->length + sizeof(msgpack_struct)) : pack->next);
            }
            memcpy(dst, pack->body, pack->length);
            return true;
        }
    }
    return false;
}

PUBLIC int recvanymsg(void *dst) {
    if (get_used_size() != 0)
        return false;
    void *ptr = get_read_ptr(); 

    msgpack_struct *pack = (msgpack_struct*)ptr;

    set_used_size(get_used_size() - pack->length + sizeof(msgpack_struct));
    set_read_ptr(pack->next == NULL ? (ptr + pack->length + sizeof(msgpack_struct)) : pack->next);

    memcpy(dst, pack->body, pack->length);
    return false;
}