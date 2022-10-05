/* 
 * SPDX-License-Identifier: LGPL-2.1-only
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
PUBLIC ASSEMBLY("_int_handlers.asm", 58) void divide_by_zero_error(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 63) void single_step_debug(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 68) void non_maskable_interrup(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 73) void breakpoint(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 78) void overflow(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 83) void bound_range_exceeded(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 88) void invalid_opcode(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 93) void device_not_available(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 98) void double_fault(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 102) void coprocessor_segment_overrun(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 107) void invalid_tss(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 111) void segment_not_present(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 115) void stack_segment_fault(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 119) void general_protection_fault(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 123) void page_fault(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 127) void reserved1_excepetion(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 132) void x87_floating_point_exception(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 137) void alignment_check(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 141) void machine_check(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 146) void simd_floating_point_exception(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 151) void virtualization_exception(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 156) void control_protection_exception(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 160) void reserved2_excepetion(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 165) void reserved3_excepetion(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 170) void reserved4_excepetion(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 175) void reserved5_excepetion(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 180) void reserved6_excepetion(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 185) void reserved7_excepetion(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 190) void hypervisor_injection_exception(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 195) void vmm_communication_exception(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 199) void security_exception(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 203) void reserved8_excepetion(void);
//IRQ处理器
PUBLIC void irq_interrup_handler(int irq);
//IRQ中断
PUBLIC ASSEMBLY("_int_handlers.asm", 243) void irq0_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 246) void irq1_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 249) void irq2_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 252) void irq3_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 255) void irq4_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 258) void irq5_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 261) void irq6_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 264) void irq7_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 267) void irq8_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 270) void irq9_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 273) void irq10_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 276) void irq11_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 279) void irq12_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 282) void irq13_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 285) void irq14_handler(void);
PUBLIC ASSEMBLY("_int_handlers.asm", 288) void irq15_handler(void);

typedef void(* irq_handler)(int);
//注册IRQ中断处理器
PUBLIC void register_irq_handler(int irq, irq_handler handler);
PUBLIC void enable_irq(int irq); //启用IRQ
PUBLIC void disable_irq(int irq); //禁用IRQ