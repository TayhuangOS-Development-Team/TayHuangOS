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
 * exception.h
 *
 * 异常处理
 *
 */




#pragma once

#include <tayhuang/defs.h>

#include <intterup/init_int.h>

//通用异常处理器
PUBLIC void general_exception_handler(int vector, struct intterup_args *regs);

//各类异常
PUBLIC void divide_by_zero_error(void); //除以0
PUBLIC void single_step_debug(void); //单步调试
PUBLIC void non_maskable_interrup(void); //NMI
PUBLIC void breakpoint(void); //断点
PUBLIC void overflow(void); //溢出
PUBLIC void bound_range_exceeded(void); //出界
PUBLIC void invalid_opcode(void); //非法指令码
PUBLIC void device_not_available(void); //设备不可用
PUBLIC void double_fault(void); //双重错误
PUBLIC void coprocessor_segment_overrun(void); //协处理器错误
PUBLIC void invalid_tss(void); //无效TSS
PUBLIC void segment_not_present(void); //段不存在
PUBLIC void stack_segment_fault(void); //栈段错误
PUBLIC void general_protection_fault(void); //通用保护错误
PUBLIC void page_fault(void); //缺页中断
PUBLIC void reserved1_excepetion(void); //保留
PUBLIC void x87_floating_point_exception(void); //x87数学协处理器浮点运算错误
PUBLIC void alignment_check(void); //对齐检测
PUBLIC void machine_check(void); //机器检测
PUBLIC void simd_floating_point_exception(void); //SIMD浮点运算错误
PUBLIC void virtualization_exception(void); //虚拟化异常
PUBLIC void control_protection_exception(void); //控制保护错误
PUBLIC void reserved2_excepetion(void); //保留
PUBLIC void reserved3_excepetion(void); //保留
PUBLIC void reserved4_excepetion(void); //保留
PUBLIC void reserved5_excepetion(void); //保留
PUBLIC void reserved6_excepetion(void); //保留
PUBLIC void reserved7_excepetion(void); //保留
PUBLIC void hypervisor_injection_exception(void); //VMM注入错误
PUBLIC void vmm_communication_exception(void); //VMM交流错误
PUBLIC void security_exception(void); //安全性错误
PUBLIC void reserved8_excepetion(void); //保留