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
 * syscall.c
 * 
 * 系统调用
 * 
 */



#include <syscall/syscall.h>
#include <syscall/syscalls.h>

#include <tayhuang/control_registers.h>

#include <task/task_scheduler.h>
#include <task/signal.h>

#include <logging.h>
#include <global.h>

PUBLIC qword syscall(int sysno, qword mode, qword counter, qword data, void *src, void *dst,
    qword arg1, qword arg2, qword arg3, qword arg4, qword arg5, qword arg6, qword arg7, qword arg8)
{
    //设置为系统页表
    __set_cr3(kernel_pml4);

    if (sysno == MOO_SN) {
        __moo();
    }
    else if (sysno == SEND_MSG_SN) {
        return __send_msg(*(msgno_id*)&arg1, src, counter, data);
    }
    else if (sysno == CHECK_IPC_SN) {
        __check_ipc();
    }
    else if (sysno == SET_ALLOW_SN) {
        __set_allow(data);
    }
    else if (sysno == RECV_MSG_SN) {
        msgpack_struct value = __recv_msg(dst);
        return *(qword *)&value; //bit hack
    }
    else if (sysno == TEST_AND_LOCK_SN) {
        return __test_and_lock(dst);
    }
    else if (sysno == SET_MAILBUFFER_SN) {
        __set_mailbuffer(dst, counter);
    }
    else if (sysno == CREATE_SIGNAL_SN) {
        return __create_signal(counter, data, arg1);
    }
    else if (sysno == GET_SIGNALS_SN) {
        return __get_signals(data);
    }
    else if (sysno == UP_SIGNAL_SN) {
        __up_signal(data);
    }
    else if (sysno == DOWN_SIGNAL_SN) {
        __down_signal(data);
    }
    else if (sysno == IS_SOFT_SIGNAL_SN) {
        return __is_soft_signal(data);
    }
    else if (sysno == REG_IRQ_SN) {
        __reg_irq(data);
    }
    else if (sysno == CREATE_THREAD_SN) {
        return __create_thread((thread_function)src, dst);
    }
    else if (sysno == EXIT_THREAD_SN) {
        __exit_thread(dst);
    }

    return 0;
}

//系统调用处理器
PUBLIC void syscall_int_handler(struct intterup_args *regs) {
    bool flag = entered_handler;
    if (! flag) {
        entered_handler = true;
    }

    //进行系统调用
    regs->rax = syscall(regs->rax, regs->rbx, regs->rcx, regs->rdx, (void *)regs->rsi, (void *)regs->rdi,
     regs->r8, regs->r9, regs->r10, regs->r11, regs->r12, regs->r13, regs->r14, regs->r15);

    if (! flag) {
        entered_handler = false;
        //没有嵌套则进行调度判断
        after_syscall(regs);
    }
}