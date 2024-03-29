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
#include <printk.h>

#include <memory/pmm.h>
#include <memory/paging.h>

#include <string.h>
#include <assert.h>

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

PRIVATE void *increase_ptr(task_struct *target, void *ptr) {
    ptr ++;
    //回到开头
    if ((target->ipc_info.mail + target->ipc_info.mail_size) <= ptr) {
        ptr = target->ipc_info.mail;
    }
    return ptr;
}

PRIVATE void write_to_buffer(task_struct *target, msgpack_struct *pack, void *src_pgd, void *src) {
    void *addr = pack;
    //写pack头
    for (int i = 0 ; i < sizeof(msgpack_struct) ; i ++) {
        *(byte *)__pa(target->mm_info.pgd, target->ipc_info.write_ptr) = *(byte *)addr;
        addr ++;
        target->ipc_info.write_ptr = increase_ptr(target, target->ipc_info.write_ptr);
    }

    //写主体
    for (int i = 0 ; i < pack->length ; i ++) {
        *(byte *)__pa(target->mm_info.pgd, target->ipc_info.write_ptr) = *(byte *)(__pa(src_pgd, src));
        src ++;
        target->ipc_info.write_ptr = increase_ptr(target, target->ipc_info.write_ptr);
    }
}

PRIVATE void read_from_buffer(msgpack_struct *pack, void *dst) {
    void *addr = pack;
    //读pack头
    for (int i = 0 ; i < sizeof(msgpack_struct) ; i ++) {
        *(byte *)addr = *(byte *)__pa(current_thread->task->mm_info.pgd, current_thread->task->ipc_info.read_ptr);
        addr ++;
        current_thread->task->ipc_info.read_ptr = increase_ptr(current_thread->task, current_thread->task->ipc_info.read_ptr);
    }

    //读主体
    for (int i = 0 ; i < pack->length ; i ++) {
        *(byte *)(__pa(current_thread->task->mm_info.pgd, dst)) = *(byte *)__pa(current_thread->task->mm_info.pgd, current_thread->task->ipc_info.read_ptr);
        dst ++;
        current_thread->task->ipc_info.read_ptr = increase_ptr(current_thread->task, current_thread->task->ipc_info.read_ptr);
    }
}

PUBLIC bool __send_msg(msgno_id msgno, void *src, qword size, int dst) {
    task_struct *target = get_task_by_pid(dst);

    if ((target == NULL) || 
        ((target->ipc_info.used_size + size + sizeof(msgpack_struct)) > target->ipc_info.mail_size) ||
        ((target->ipc_info.allow_pid != current_thread->task->pid) && (target->ipc_info.allow_pid != ANY_TASK)))
    {
        return false;
    }

    msgpack_struct pack;
    pack.length = size;
    pack.message_no = msgno.message_no;
    pack.msg_id = msgno.msg_id;
    pack.source = current_thread->task->pid;

    //增加已使用大小
    target->ipc_info.used_size += sizeof(msgpack_struct) + size;

    //写到buffer里
    write_to_buffer(target, &pack, current_thread->task->mm_info.pgd, src);

    if (target->ipc_info.msg_handler_thread->state == WAITING_IPC && target->ipc_info.allow_pid != DUMMY_TASK) {
        target->ipc_info.msg_handler_thread->state = READY;

        enqueue_thread(target->ipc_info.msg_handler_thread);
    }
    return true;
}

PUBLIC bool send_msg(msgno_id msgno, void *src, qword size, int dst) {
    return dosyscall(SEND_MSG_SN, 0, size, dst, src, NULL, *(qword*)&msgno, 0, 0, 0, 0, 0, 0, 0);
}

//-------------------

PUBLIC void __check_ipc(void) {
    if (current_thread->task->ipc_info.mail == NULL) {
        return;
    }

    if (current_thread->task->ipc_info.used_size > 0) {
        return;
    }

    current_thread->state = WAITING_IPC;
}

PUBLIC void check_ipc(void) {
    dosyscall(CHECK_IPC_SN, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

//-------------------

PUBLIC void __set_allow(int pid) {
    current_thread->task->ipc_info.allow_pid = pid;
}

PUBLIC void set_allow(int pid) {
    dosyscall(SET_ALLOW_SN, 0, 0, pid, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

//------------------

PUBLIC msgpack_struct __recv_msg(void *dst) {
    if (current_thread->task->ipc_info.used_size <= 0) {
        return (msgpack_struct){.source = -1, .message_no = -1};
    }

    msgpack_struct pack;
    //读取
    read_from_buffer(&pack, dst);
    
    //减少已使用大小
    assert(current_thread->task->ipc_info.used_size >= (pack.length + sizeof(msgpack_struct)));
    current_thread->task->ipc_info.used_size -= (pack.length + sizeof(msgpack_struct));

    return pack;
}

PUBLIC msgpack_struct recv_msg(void *dst) {
    qword value = dosyscall(RECV_MSG_SN, 0, 0, 0, NULL, dst, 0, 0, 0, 0, 0, 0, 0, 0);;
    return *(msgpack_struct *)&value;
}
//--------------------

PUBLIC bool __test_and_lock(bool *val) {
    bool *paddr = (bool *)__pa(current_thread->task->mm_info.pgd, val);

    if (*paddr) {
        return false;
    }

    *paddr = true;
    return true;
}

PUBLIC bool test_and_lock(bool *val) {
    return dosyscall(TEST_AND_LOCK_SN, 0, 0, 0, NULL, val, 0, 0, 0, 0, 0, 0, 0, 0);
}

//-------------------

PUBLIC void __set_mailbuffer(void *buffer, qword size) {
    //设置指针
    current_thread->task->ipc_info.read_ptr = current_thread->task->ipc_info.write_ptr = current_thread->task->ipc_info.mail = buffer;
    //设置邮箱大小
    current_thread->task->ipc_info.mail_size = size;
    //设置已使用大小
    current_thread->task->ipc_info.used_size = 0;
    //设置最新消息
    current_thread->task->ipc_info.lastest_msg = NULL;

    current_thread->task->ipc_info.msg_handler_thread = current_thread;
}

PUBLIC void set_mailbuffer(void *buffer, qword size) {
    dosyscall(SET_MAILBUFFER_SN, 0, size, 0, NULL, buffer, 0, 0, 0, 0, 0, 0, 0, 0);
}

//--------------------

PUBLIC void __clear_mail(void) {
    //指针归零
    current_thread->task->ipc_info.read_ptr = current_thread->task->ipc_info.write_ptr = current_thread->task->ipc_info.mail;
    //已使用大小归零
    current_thread->task->ipc_info.used_size = 0;
    //最新消息清空
    current_thread->task->ipc_info.lastest_msg = NULL;
}

PUBLIC void clear_mail(void) {
    dosyscall(CLEAR_MAIL_SN, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
}

//--------------------

PRIVATE int IRQ_HANDLE_TASKS[16] = {};

PUBLIC void normal_irq_handler(int irq, struct intterup_args *args, bool flags) {
    if (IRQ_HANDLE_TASKS[irq] == 0) {
        return;
    }
    dummy_send_msg((msgno_id){.message_no = MSG_IRQ, .msg_id = get_msgid()}, &irq, sizeof(int), IRQ_HANDLE_TASKS[irq]);
}

//--------------------

PUBLIC id_t create_signal(int max_signals, int value, bool soft) {
    return dosyscall(CREATE_SIGNAL_SN, 0, max_signals, value, NULL, NULL, soft, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC int get_signals(id_t id) {
    return dosyscall(GET_SIGNALS_SN, 0, 0, id, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC void up_signal(id_t id) {
    dosyscall(UP_SIGNAL_SN, 0, 0, id, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC void down_signal(id_t id) {
    dosyscall(DOWN_SIGNAL_SN, 0, 0, id, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC bool is_soft_signal(id_t id) {
    return dosyscall(IS_SOFT_SIGNAL_SN, 0, 0, id, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

//--------------------

PUBLIC void __reg_irq(int irq) {
    IRQ_HANDLE_TASKS[irq] = current_thread->task->pid;
}

PUBLIC void reg_irq(int irq) {
    dosyscall(REG_IRQ_SN, 0, 0, irq, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0);
}

//--------------------

PUBLIC bool dummy_send_msg(msgno_id msgno, void *src, qword size, int dst) {
    task_struct *target = get_task_by_pid(dst);

    if ((target == NULL) || 
        ((target->ipc_info.used_size + size + sizeof(msgpack_struct)) > target->ipc_info.mail_size) ||
        ((target->ipc_info.allow_pid != DUMMY_TASK) && (target->ipc_info.allow_pid >= 0)))
    {
        return false;
    }

    msgpack_struct pack;
    pack.message_no = msgno.message_no;
    pack.msg_id = msgno.msg_id;
    pack.length = size;
    pack.source = DUMMY_TASK;

    //增加已使用大小
    target->ipc_info.used_size += sizeof(msgpack_struct) + size;

    //写到buffer里
    write_to_buffer(target, &pack, kernel_pml4, src);

    //唤醒
    if (target->ipc_info.msg_handler_thread->state == WAITING_IPC) {
        target->ipc_info.msg_handler_thread->state = READY;

        enqueue_thread(target->ipc_info.msg_handler_thread);
    }
    return true;
}

//-------------------

PUBLIC int __create_thread(thread_function entry, void *args) {
    thread_info_struct *thread = __create_thread__(entry, current_thread->task);
    thread->registers.basic.rdi = (qword)args; //参数

    return thread->tid;
}

PUBLIC int create_thread(thread_function entry, void *args) {
    return dosyscall(CREATE_THREAD_SN, 0, 0, 0, entry, args, 0, 0, 0, 0, 0, 0, 0, 0);
}

PUBLIC void __exit_thread(void *retval) {
    //TODO: save the retval
    current_thread->state = TERMINATED;
}

PUBLIC void exit_thread(void *retval) {
    dosyscall(EXIT_THREAD_SN, 0, 0, 0, NULL, retval, 0, 0, 0, 0, 0, 0, 0, 0);
}