/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * intcall.h
 *
 * 中断调用函数
 *
 */



#pragma once

#include "header.h"

//对eflags的各种位的操作
#define EF_SETCF(eflags) (eflags |= (1 << 0))
#define EF_CLRCF(eflags) (eflags &= (~(1 << 0)))
#define EF_GETCF(eflags) (eflags & (1 << 0))
#define EF_SETPF(eflags) (eflags |= (1 << 2))
#define EF_CLRPF(eflags) (eflags &= (~(1 << 2)))
#define EF_GETPF(eflags) (eflags & (1 << 2))
#define EF_SETAF(eflags) (eflags |= (1 << 4))
#define EF_CLRAF(eflags) (eflags &= (1 << 4))
#define EF_GETAF(eflags) (eflags & (1 << 4))
#define EF_SETZF(eflags) (eflags |= (1 << 6))
#define EF_CLRZF(eflags) (eflags &= (1 << 6))
#define EF_GETZF(eflags) (eflags & (1 << 6))
#define EF_SETSF(eflags) (eflags |= (1 << 7))
#define EF_CLRSF(eflags) (eflags &= (1 << 7))
#define EF_GETSF(eflags) (eflags & (1 << 7)
#define EF_SETTF(eflags) (eflags |= (1 << 8))
#define EF_CLRTF(eflags) (eflags &= (1 << 8))
#define EF_GETTF(eflags) (eflags & (1 << 8)
#define EF_SETIF(eflags) (eflags |= (1 << 9))
#define EF_CLRIF(eflags) (eflags &= (1 << 9))
#define EF_GETIF(eflags) (eflags & (1 << 9)
#define EF_SETDF(eflags) (eflags |= (1 << 10))
#define EF_CLRDF(eflags) (eflags &= (1 << 10))
#define EF_GETDF(eflags) (eflags & (1 << 10)
#define EF_SETOF(eflags) (eflags |= (1 << 11))
#define EF_CLROF(eflags) (eflags &= (1 << 11))
#define EF_GETOF(eflags) (eflags & (1 << 11))
#define EF_SETIOPL(eflags) (eflags |= (3 << 12))
#define EF_CLRIOPL(eflags) (eflags &= (3 << 12))
#define EF_GETIOPL(eflags) (eflags & (3 << 12))
#define EF_SETNT(eflags) (eflags |= (1 << 14))
#define EF_CLRNT(eflags) (eflags &= (1 << 14))
#define EF_GETNT(eflags) (eflags & (1 << 14))
#define EF_SETRF(eflags) (eflags |= (1 << 16))
#define EF_CLRRF(eflags) (eflags &= (1 << 16))
#define EF_GETRF(eflags) (eflags & (1 << 16))
#define EF_SETVM(eflags) (eflags |= (1 << 17))
#define EF_CLRVM(eflags) (eflags &= (1 << 17))
#define EF_GETVM(eflags) (eflags & (1 << 17))
#define EF_SETAC(eflags) (eflags |= (1 << 18))
#define EF_CLRAC(eflags) (eflags &= (1 << 18))
#define EF_GETAC(eflags) (eflags & (1 << 18))
#define EF_SETVIF(eflags) (eflags |= (1 << 19))
#define EF_CLRVIF(eflags) (eflags &= (1 << 19))
#define EF_GETVIF(eflags) (eflags & (1 << 19))
#define EF_SETVIP(eflags) (eflags |= (1 << 20))
#define EF_CLRVIP(eflags) (eflags &= (1 << 20))
#define EF_GETVIP(eflags) (eflags & (1 << 20))
#define EF_SETTD(eflags) (eflags |= (1 << 21))
#define EF_CLRTD(eflags) (eflags &= (1 << 21))
#define EF_GETTD(eflags) (eflags & (1 << 21))

//寄存器集合
typedef struct {
    reg32_t eax;
    reg32_t ebx;
    reg32_t ecx;
    reg32_t edx;
    reg32_t edi;
    reg32_t esi;
    sreg_t es;
    sreg_t fs;
    sreg_t gs;
    reg16_t eflags;
}reg_collect_t, *preg_collect;

//中断参数
typedef struct {
    preg_collect in_regs;
    preg_collect out_regs;
    int int_no;
}intargs_t, *pintargs;

//中断函数
PUBLIC void intcall(pintargs args);