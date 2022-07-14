/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * int_handlers.h
 *
 * 中断处理函数
 *
 */



#pragma once

#include <tayhuang/defs.h>

//异常处理器
PUBLIC void exception_handler(int vector_no, int errcode, int eip, int cs, int eflags);
//异常
PUBLIC void divide_by_zero_error(void);
PUBLIC void single_step_debug(void);
PUBLIC void non_maskable_interrup(void);
PUBLIC void breakpoint(void);
PUBLIC void overflow(void);
PUBLIC void bound_range_exceeded(void);
PUBLIC void invalid_opcode(void);
PUBLIC void device_not_available(void);
PUBLIC void double_fault(void);
PUBLIC void coprocessor_segment_overrun(void);
PUBLIC void invalid_tss(void);
PUBLIC void segment_not_present(void);
PUBLIC void stack_segment_fault(void);
PUBLIC void general_protection_fault(void);
PUBLIC void page_fault(void);
PUBLIC void reserved1_excepetion(void);
PUBLIC void x87_floating_point_exception(void);
PUBLIC void alignment_check(void);
PUBLIC void machine_check(void);
PUBLIC void simd_floating_point_exception(void);
PUBLIC void virtualization_exception(void);
PUBLIC void control_protection_exception(void);
PUBLIC void reserved2_excepetion(void);
PUBLIC void reserved3_excepetion(void);
PUBLIC void reserved4_excepetion(void);
PUBLIC void reserved5_excepetion(void);
PUBLIC void reserved6_excepetion(void);
PUBLIC void reserved7_excepetion(void);
PUBLIC void hypervisor_injection_exception(void);
PUBLIC void vmm_communication_exception(void);
PUBLIC void security_exception(void);
PUBLIC void reserved8_excepetion(void);
//IRQ处理器
PUBLIC void irq_interrup_handler(int irq);
//IRQ中断
PUBLIC void irq0_handler(void);
PUBLIC void irq1_handler(void);
PUBLIC void irq2_handler(void);
PUBLIC void irq3_handler(void);
PUBLIC void irq4_handler(void);
PUBLIC void irq5_handler(void);
PUBLIC void irq6_handler(void);
PUBLIC void irq7_handler(void);
PUBLIC void irq8_handler(void);
PUBLIC void irq9_handler(void);
PUBLIC void irq10_handler(void);
PUBLIC void irq11_handler(void);
PUBLIC void irq12_handler(void);
PUBLIC void irq13_handler(void);
PUBLIC void irq14_handler(void);
PUBLIC void irq15_handler(void);

typedef void(*irq_handler)(int);
//注册IRQ中断处理器
PUBLIC void register_irq_handler(int irq, irq_handler handler);
PUBLIC void enable_irq(int irq); //启用IRQ
PUBLIC void disable_irq(int irq); //禁用IRQ