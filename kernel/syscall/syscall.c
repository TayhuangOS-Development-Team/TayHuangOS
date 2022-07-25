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

#include <logging.h>
#include <global.h>

PUBLIC qword syscall(int sysno, qword mode, qword counter, qword data, void *src, void *dst,
    qword arg1, qword arg2, qword arg3, qword arg4, qword arg5, qword arg6, qword arg7, qword arg8)
{
    //设置为系统页表
    __set_cr3(kernel_pml4);
    //选择合适的函数
    switch (sysno)
    {
    case MOO_SN: __moo(); break;
    case SEND_MSG_SN: return __send_msg(arg1, src, counter, data);
    case CHECK_IPC_SN: __check_ipc(); break;
    case SET_ALLOW_SN: __set_allow(data); break;
    case RECV_MSG_SN: {
        recvmsg_result_struct value = __recv_msg(dst);
        return *(qword *)&value; //bit hack
    }
    case SET_MAILBUFFER_SN: __set_mailbuffer(dst, counter); break;
    case REG_IRQ_SN: __reg_irq(data); break;
    case TEST_AND_LOCK_SN: __test_and_lock(dst); break;
    default: break;
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