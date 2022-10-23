/**
 * @file intterup.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 中断
 * @version alpha-1.0.0
 * @date 2022-10-23
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */



#pragma once

#include <tayhuang/attributes.h>
#include <tayhuang/descs.h>

//异常
ASSEMBLY PUBLIC void divide_by_zero_error(void);
ASSEMBLY PUBLIC void single_step_debug(void);
ASSEMBLY PUBLIC void non_maskable_interrup(void);
ASSEMBLY PUBLIC void breakpoint(void);
ASSEMBLY PUBLIC void overflow(void);
ASSEMBLY PUBLIC void bound_range_exceeded(void);
ASSEMBLY PUBLIC void invalid_opcode(void);
ASSEMBLY PUBLIC void device_not_available(void);
ASSEMBLY PUBLIC void double_fault(void);
ASSEMBLY PUBLIC void coprocessor_segment_overrun(void);
ASSEMBLY PUBLIC void invalid_tss(void);
ASSEMBLY PUBLIC void segment_not_present(void);
ASSEMBLY PUBLIC void stack_segment_fault(void);
ASSEMBLY PUBLIC void general_protection_fault(void);
ASSEMBLY PUBLIC void page_fault(void);
ASSEMBLY PUBLIC void reserved1_excepetion(void);
ASSEMBLY PUBLIC void x87_floating_point_exception(void);
ASSEMBLY PUBLIC void alignment_check(void);
ASSEMBLY PUBLIC void machine_check(void);
ASSEMBLY PUBLIC void simd_floating_point_exception(void);
ASSEMBLY PUBLIC void virtualization_exception(void);
ASSEMBLY PUBLIC void control_protection_exception(void);
ASSEMBLY PUBLIC void reserved2_excepetion(void);
ASSEMBLY PUBLIC void reserved3_excepetion(void);
ASSEMBLY PUBLIC void reserved4_excepetion(void);
ASSEMBLY PUBLIC void reserved5_excepetion(void);
ASSEMBLY PUBLIC void reserved6_excepetion(void);
ASSEMBLY PUBLIC void reserved7_excepetion(void);
ASSEMBLY PUBLIC void hypervisor_injection_exception(void);
ASSEMBLY PUBLIC void vmm_communication_exception(void);
ASSEMBLY PUBLIC void security_exception(void);
ASSEMBLY PUBLIC void reserved8_excepetion(void);
//IRQ中断
ASSEMBLY PUBLIC void irq0_handler(void);
ASSEMBLY PUBLIC void irq1_handler(void);
ASSEMBLY PUBLIC void irq2_handler(void);
ASSEMBLY PUBLIC void irq3_handler(void);
ASSEMBLY PUBLIC void irq4_handler(void);
ASSEMBLY PUBLIC void irq5_handler(void);
ASSEMBLY PUBLIC void irq6_handler(void);
ASSEMBLY PUBLIC void irq7_handler(void);
ASSEMBLY PUBLIC void irq8_handler(void);
ASSEMBLY PUBLIC void irq9_handler(void);
ASSEMBLY PUBLIC void irq10_handler(void);
ASSEMBLY PUBLIC void irq11_handler(void);
ASSEMBLY PUBLIC void irq12_handler(void);
ASSEMBLY PUBLIC void irq13_handler(void);
ASSEMBLY PUBLIC void irq14_handler(void);
ASSEMBLY PUBLIC void irq15_handler(void);

//异常处理器
PUBLIC void exception_handler(int vector_no, int errcode, int eip, int cs, int eflags);
//IRQ处理器
PUBLIC void irq_interrup_handler(int irq);

typedef void(* irq_handler)(int);
//设置IRQ中断处理器
PUBLIC void set_handler(int irq, irq_handler handler);
PUBLIC void enable_irq(int irq); //启用IRQ
PUBLIC void disable_irq(int irq); //禁用IRQ

#define IDT_SIZE (128)

EXTERN PUBLIC gate_desc IDT[IDT_SIZE];
EXTERN PUBLIC struct desc_ptr idtr;

typedef void(* int_handler)(void);
//设置IDT
PUBLIC void init_pic(void);
PUBLIC void init_idt_desc(byte vector, byte type, int_handler handler, byte privilege);
PUBLIC void init_idt(void);