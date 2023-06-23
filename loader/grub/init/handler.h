/**
 * @file handler.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 中断处理器
 * @version alpha-1.0.0
 * @date 2023-06-07
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <init/idt.h>

/**
 * @brief 堆栈结构
 * 
 */
typedef struct {
    b32 edi,
        esi,
        edx,
        ecx,
        ebx,
        eax,
        gs,
        fs,
        es,
        ds,
        cr3,
        ebp,
        handler_esp,
        eip,
        cs,
        eflags,
        esp,
        ss;
} istack_t;

/** 中断处理器 */
typedef bool(*irq_handler_t)(int irq, istack_t *stack);

extern irq_handler_t irq_handlers[32];

/**
 * @brief IRQ主处理程序
 * 
 * @param irq irq号
 * @param stack 堆栈
 */
void primary_irq_handler(int irq, istack_t *stack);

/**
 * @brief 异常主处理程序
 * 
 * @param errno 异常号
 * @param stack 堆栈
 */
void primary_exception_handler(int errno, istack_t *stack);

//------------------------------------------

void divide_by_zero_error(void); //除以0
void single_step_debug(void); //单步调试
void non_maskable_interrup(void); //NMI
void breakpoint(void); //断点
void overflow(void); //溢出
void bound_range_exceeded(void); //出界
void invalid_opcode(void); //非法指令码
void device_not_available(void); //设备不可用
void double_fault(void); //双重错误
void coprocessor_segment_overrun(void); //协处理器错误
void invalid_tss(void); //无效TSS
void segment_not_present(void); //段不存在
void stack_segment_fault(void); //栈段错误
void general_protection_fault(void); //通用保护错误
void page_fault(void); //缺页中断
void reserved1_excepetion(void); //
void x87_floating_point_exception(void); //x87数学协处理器浮点运算错误
void alignment_check(void); //对齐检测
void machine_check(void); //机器检测
void simd_floating_point_exception(void); //SIMD浮点运算错误
void virtualization_exception(void); //虚拟化异常
void control_protection_exception(void); //控制保护错误
void reserved2_excepetion(void); //
void reserved3_excepetion(void); //
void reserved4_excepetion(void); //
void reserved5_excepetion(void); //
void reserved6_excepetion(void); //
void reserved7_excepetion(void); //
void hypervisor_injection_exception(void); //VMM注入错误
void vmm_communication_exception(void); //VMM交流错误
void security_exception(void); //安全性错误
void reserved8_excepetion(void); //

//------------------------------------------

void irq0_handler(void);
void irq1_handler(void);
void irq2_handler(void);
void irq3_handler(void);
void irq4_handler(void);
void irq5_handler(void);
void irq6_handler(void);
void irq7_handler(void);
void irq8_handler(void);
void irq9_handler(void);
void irq10_handler(void);
void irq11_handler(void);
void irq12_handler(void);
void irq13_handler(void);
void irq14_handler(void);
void irq15_handler(void);