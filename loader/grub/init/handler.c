/**
 * @file handler.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 中断处理器
 * @version alpha-1.0.0
 * @date 2023-06-07
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <init/handler.h>
#include <primlib/logger.h>
#include <stddef.h>
#include <tay/ports.h>
#include <tay/io.h>

//禁用/启用 IRQ
static void disable_irq(int irq) {
    if (irq < 8) {
        //主片
        byte i = inb(M_PIC_BASE + PIC_DATA); 
        i |= (1 << irq);
        outb(M_PIC_BASE + PIC_DATA, i);
    }
    else {
        //从片
        byte i = inb(S_PIC_BASE + PIC_DATA); 
        i |= (1 << (irq - 8));
        outb(S_PIC_BASE + PIC_DATA, i);
    }
}

static void enable_irq(int irq) {
    if (irq < 8) {
        //主片
        byte i = inb(M_PIC_BASE + PIC_DATA); 
        i &= ~(1 << irq);
        outb(M_PIC_BASE + PIC_DATA, i);
    }
    else {
        //从片
        byte i = inb(S_PIC_BASE + PIC_DATA); 
        i &= ~(1 << (irq - 8));
        outb(S_PIC_BASE + PIC_DATA, i);
    }
}

#define PIC_EOI (0x20)

//发送EOI
static void send_eoi(int irq) {
    if (irq > 8) {
        //从片EOI
        outb (S_PIC_BASE + PIC_CONTROL, PIC_EOI); 
    }
    //主片EOI
    outb (M_PIC_BASE + PIC_CONTROL, PIC_EOI); 
}

static int ticks = 0;

bool clock_handler(int irq, istack_t *stack) {
    ticks ++;
    log_info("Ticks=%d", ticks);

    return false;
}

/**
 * @brief IRQ处理器
 * 
 */
irq_handler_t irq_handlers[32] = {
    [0] = clock_handler
};

/**
 * @brief IRQ主处理程序
 * 
 * @param irq irq号
 * @param stack 堆栈
 */
void primary_irq_handler(int irq, istack_t *stack) {
    disable_irq(irq); 

    //发送EOI
    send_eoi(irq);

    log_info("接收到IRQ=%02X", irq);

    if (irq_handlers[irq] != NULL) {
        if (! irq_handlers[irq](irq, stack)) {
            log_error("解决IRQ=%02X失败!", irq);
            return; //不再开启该中断
        }
    }

    //启用同类中断接收
    enable_irq(irq); 
}

//---------------------------------------------

int errcode;

static const char *exception_msg[] = { //异常信息
    "[#DE] 除以0!",
    "[#DB] 单步调试",
    "[无] NMI中断!",
    "[#BP] 断点",
    "[#OF] 溢出!",
    "[#BR] 越界!",
    "[#UD] 无效的操作码(未定义的指令)!",
    "[#NM] 设备不可用(没有数学协处理器)!",
    "[#DF] 双重错误!",
    "[无] 协处理器段溢出!",
    "[#TS] 无效TSS!",
    "[#NP] 缺少段!",
    "[#SS] 缺少栈段!",
    "[#GP] 通用保护错误!",
    "[#PF] 缺页中断!",
    "[保留] 保留!",
    "[#MF] x87数学协处理器浮点运算错误!",
    "[#AC] 对齐检测!",
    "[#MC] 机器检测!",
    "[#XF] SIMD浮点运算错误!",
    "[#VE] 虚拟化异常!",
    "[#CP] 控制保护错误!",
    "[保留] 保留!",
    "[保留] 保留!",
    "[保留] 保留!",
    "[保留] 保留!",
    "[保留] 保留!",
    "[保留] 保留!",
    "[#HV] Hypervisor注入异常!",
    "[#VC] VMM通信异常!",
    "[#SX] 安全性错误!",
    "[保留] 保留!"
};

typedef bool(*exception_solution_t)(void);

static const exception_solution_t solution_list[] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

/**
 * @brief 异常主处理程序
 * 
 * @param errno 异常号
 * @param stack 堆栈
 */
void primary_exception_handler(int errno, istack_t *stack) {
    log_error("在%04X:%08X处发生错误:", stack->cs, stack->eip);
    log_error("%s", exception_msg[errno]);

    if (errcode != 0xFFFFFFFF) {
        log_error("Error Code = %08X", errcode);
    }

    log_error("现场已保存:");

    log_error("eax: %08X ; ebx: %08X ; ecx: %08X ; edx: %08X", stack->eax, stack->ebx, stack->ecx, stack->edx);
    log_error("esi: %08X ; edi: %08X ; esp: %08X ; ebp: %08X", stack->esi, stack->edi, stack->esp, stack->ebp);

    log_error(" ds: %04X     ;  es: %04X     ;  fs: %04X     ;  gs: %04X    ", stack->ds, stack->es, stack->fs, stack->gs);

    log_error("cr3: %08X ; eflags: %08X", stack->cr3, stack->eflags);

    if (solution_list[errno] != NULL) {
        if (! solution_list[errno]()) {
            log_fatal("解决异常%02X失败!", errno);
            while (true);
        }
    }
    else {
        log_fatal("无法解决异常%02X!", errno);
        while (true);
    }
}