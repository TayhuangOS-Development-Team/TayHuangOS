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
        handlerEsp,
        eip,
        cs,
        eflags,
        esp,
        ss;
} IStack;

/** 中断处理器 */
typedef bool(*IRQHandler)(int irq, IStack *stack);

extern IRQHandler irqHandlers[32];

/**
 * @brief IRQ主处理程序
 * 
 * @param irq irq号
 * @param stack 堆栈
 */
void PrimaryIRQHandler(int irq, IStack *stack);

/**
 * @brief 异常主处理程序
 * 
 * @param errno 异常号
 * @param stack 堆栈
 */
void PrimaryExceptionHandler(int errno, IStack *stack);

/**
 * @brief 启用中断
 * 
 */
void EnableInterrupt(void);

/**
 * @brief 禁止中断
 * 
 */
void DisableInterrupt(void);

//------------------------------------------

void DivideByZeroFaultHandler(void); //除以0
void SingleStepTrapHandler(void); //单步调试
void NMIHandler(void); //NMI
void BreakpointTrapHandler(void); //断点
void OverflowTrapHandler(void); //溢出
void BoundRangeExceededFaultHandler(void); //出界
void InvalidOpcodeFaultHandler(void); //非法指令码
void DeviceNotAvailableFaultHandler(void); //设备不可用
void DoubleFaultHandler(void); //双重错误
void CoprocessorSegmentOverrunFaultHandler(void); //协处理器错误
void InvalidTSSFault(void); //无效TSS
void SegmentNotPresentFaultHandler(void); //段不存在
void StackSegmentFaultHandler(void); //栈段错误
void GeneralProtectionFaultHandler(void); //通用保护错误
void PageFaultHandler(void); //缺页中断
void ReservedHandler1(void); //
void X87FloatingPointFaultHandler(void); //x87数学协处理器浮点运算错误
void AlignmentCheckHandler(void); //对齐检测
void MachineCheckHandler(void); //机器检测
void SIMDFloatingPointFaultHandler(void); //SIMD浮点运算错误
void VirtualizationFaultHandler(void); //虚拟化异常
void ControlProtectionFaultHandler(void); //控制保护错误
void ReservedHandler2(void); //
void ReservedHandler3(void); //
void ReservedHandler4(void); //
void ReservedHandler5(void); //
void ReservedHandler6(void); //
void ReservedHandler7(void); //
void HypervisorInjectionException(void); //VMM注入错误
void VMMCommunicationFaultHandler(void); //VMM交流错误
void SecurityFaultHandler(void); //安全性错误
void ReservedHandler8(void); //

//------------------------------------------

void Irq0Handler(void);
void Irq1Handler(void);
void Irq2Handler(void);
void Irq3Handler(void);
void Irq4Handler(void);
void Irq5Handler(void);
void Irq6Handler(void);
void Irq7Handler(void);
void Irq8Handler(void);
void Irq9Handler(void);
void Irq10Handler(void);
void Irq11Handler(void);
void Irq12Handler(void);
void Irq13Handler(void);
void Irq14Handler(void);
void Irq15Handler(void);