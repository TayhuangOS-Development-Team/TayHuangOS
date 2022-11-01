/**
 * @file intterup.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 中断
 * @version alpha-1.0.0
 * @date 2022-10-23
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */



#include <init/intterup.h>

#include <tayhuang/ports.h>
#include <tayhuang/io.h>

#include <logging.h>

PUBLIC void exception_handler(int vector_no, int errcode, int eip, int cs, int eflags) {
    const char *exception_msg[] = { //异常信息
        "[#DE] Divide by 0 error!\n",
        "[#DB] Single step\n",
        "[None] Non-maskable interrup!\n",
        "[#BP] Break point\n",
        "[#OF] Overflow!\n",
        "[#BR] Bound range exceeded!\n",
        "[#UD] Invalid Opcode(Undefined Opcode)!\n",
        "[#NM] Device not available(No math coprocessor)!\n",
        "[#DF] Double fault!\n",
        "[None] Coprocessor segment overrun(Should be reserved)!\n",
        "[#TS] Invalid TSS!\n",
        "[#NP] Segment not present!\n",
        "[#SS] Stack segment not present!\n",
        "[#GP] General protection fault!\n",
        "[#PF] Page fault!\n",
        "[Reserved] Reserved exception!\n",
        "[#MF] x87 floating-point exception(Math fault)!\n",
        "[#AC] Alignment check!\n",
        "[#MC] Machine check!\n",
        "[#XF] SIMD floating-point exception!\n",
        "[#VE] Virtualization exception!\n",
        "[#CP] Control protection exception!\n",
        "[Reserved] Reserved exception!\n",
        "[Reserved] Reserved exception!\n",
        "[Reserved] Reserved exception!\n",
        "[Reserved] Reserved exception!\n",
        "[Reserved] Reserved exception!\n",
        "[Reserved] Reserved exception!\n",
        "[#HV] Hypervisor injection exception!\n",
        "[#VC] VMM communication exception!\n",
        "[#SX] Security exception!\n", 
        "[Reserved] Reserved exception!\n"
    };

    LFATAL ("Exception Handler", "Oops!There is a exception throwing!The followings are its information:"); //
    if (vector_no < 32) {
        LFATALF ("Exception Handler", "Message: %s", exception_msg[vector_no]); //打印信息
    }
    LFATALF ("Exception Handler", "Vector = %d ; Error code = %d", vector_no, errcode); //打印Vector号

    LFATAL ("Exception Handler", "Registers:");
    int _eax, _ebx, _ecx, _edx, _esi, _edi, _esp, _ebp;
    asmv ("movl %%eax, %0" : "=m"(_eax));
    asmv ("movl %%ebx, %0" : "=m"(_ebx));
    asmv ("movl %%ecx, %0" : "=m"(_ecx));
    asmv ("movl %%edx, %0" : "=m"(_edx));
    asmv ("movl %%esi, %0" : "=m"(_esi));
    asmv ("movl %%edi, %0" : "=m"(_edi));
    asmv ("movl %%esp, %0" : "=m"(_esp));
    asmv ("movl %%ebp, %0" : "=m"(_ebp)); //获取寄存器
    LFATALF ("Exception Handler", "eax: %#08X ; ebx: %#08X", _eax, _ebx);
    LFATALF ("Exception Handler", "ecx: %#08X ; edx: %#08X", _ecx, _edx);
    LFATALF ("Exception Handler", "esi: %#08X ; edi: %#08X", _esi, _edi);
    LFATALF ("Exception Handler", "esp: %#08X ; ebp: %#08X", _esp, _ebp);
    LFATALF ("Exception Handler", "cs:  %#04X     ;  ds: %#04X", cs, rdds());
    LFATALF ("Exception Handler", "eip: %#08X;eflags:%#08X", eip, eflags); //打印寄存器

    while (true);
}

PRIVATE irq_handler IRQ_HANDLER_LIST[16] = {};

PUBLIC void set_handler(int irq, irq_handler handler) {
    IRQ_HANDLER_LIST[irq] = handler;
}

PUBLIC void enable_irq(int irq) {
    if (irq < 8) {
        byte i = inb(M_PIC_DATA); //主片
        i &= ~(1 << irq);
        outb(M_PIC_DATA, i);
    }
    else {
        byte i = inb(S_PIC_DATA); //从片
        i &= ~(1 << (irq - 8));
        outb(S_PIC_DATA, i);
    }
}

PUBLIC void disable_irq(int irq) {
    if (irq < 8) {
        byte i = inb(M_PIC_DATA); //主片
        i |= (1 << irq);
        outb(M_PIC_DATA, i);
    }
    else {
        byte i = inb(S_PIC_DATA); //从片
        i |= (1 << (irq - 8));
        outb(S_PIC_DATA, i);
    }
}

PUBLIC void irq_interrup_handler(int irq) {
    disable_irq(irq); //禁用IRQ

    outb(M_PIC_CONTROL, 0x20); //EOI
    outb(DELAY_PORT, 0);
    outb(S_PIC_CONTROL, 0x20); //EOI

    asmv ("sti"); //CPU会自动关中断

    if (IRQ_HANDLER_LIST[irq] != NULL) { //存在对应的IRQ处理器
        IRQ_HANDLER_LIST[irq](irq);
    }

    asmv ("cli"); //恢复原来状态

    enable_irq(irq); //启用IRQ
}

gate_desc IDT[IDT_SIZE];
struct desc_ptr idtr;

#define START_VECTOR (32)

PUBLIC void init_pic(void) {
    outb (M_PIC_CONTROL, 0x11);
    outb (S_PIC_CONTROL, 0x11); //ICW1

    outb (M_PIC_DATA, START_VECTOR + 0);
    outb (S_PIC_DATA, START_VECTOR + 8); //ICW2

    outb (M_PIC_DATA, 0x4);
    outb (S_PIC_DATA, 0x2); //ICW3

    outb (M_PIC_DATA, 0x1);
    outb (S_PIC_DATA, 0x1); //ICW4

    outb (M_PIC_DATA, 0xFF); //OCW1
    outb (S_PIC_DATA, 0xFF);
}

PUBLIC void init_idt_desc(byte vector, byte type, int_handler handler, byte privilege) { //初始化一个IDT描述符
    dword base = (dword)handler;
    IDT[vector].offset_low = base & 0xFFFF; //偏移
    IDT[vector].segment = rdcs(); //段
    IDT[vector].bits.ist = 0;
    IDT[vector].bits.zero = 0;
    IDT[vector].bits.type = type; //类型
    IDT[vector].bits.dpl = privilege; //权限
    IDT[vector].bits.p = 1; //存在
    IDT[vector].offset_middle = base >> 16; //偏移
}

PRIVATE void _int_idt_descs(void) {
    //异常的IDT
    init_idt_desc(0, GATE_INTERRUPT, divide_by_zero_error, 0);
    init_idt_desc(1, GATE_INTERRUPT, single_step_debug, 0);
    init_idt_desc(2, GATE_INTERRUPT, non_maskable_interrup, 0);
    init_idt_desc(3, GATE_INTERRUPT, breakpoint, 0);
    init_idt_desc(4, GATE_INTERRUPT, overflow, 0);
    init_idt_desc(5, GATE_INTERRUPT, bound_range_exceeded, 0);
    init_idt_desc(6, GATE_INTERRUPT, invalid_opcode, 0);
    init_idt_desc(7, GATE_INTERRUPT, device_not_available, 0);
    init_idt_desc(8, GATE_INTERRUPT, double_fault, 0);
    init_idt_desc(9, GATE_INTERRUPT, coprocessor_segment_overrun, 0);
    init_idt_desc(10, GATE_INTERRUPT, invalid_tss, 0);
    init_idt_desc(11, GATE_INTERRUPT, segment_not_present, 0);
    init_idt_desc(12, GATE_INTERRUPT, stack_segment_fault, 0);
    init_idt_desc(13, GATE_INTERRUPT, general_protection_fault, 0);
    init_idt_desc(14, GATE_INTERRUPT, page_fault, 0);
    init_idt_desc(15, GATE_INTERRUPT, reserved1_excepetion, 0);
    init_idt_desc(16, GATE_INTERRUPT, x87_floating_point_exception, 0);
    init_idt_desc(17, GATE_INTERRUPT, alignment_check, 0);
    init_idt_desc(18, GATE_INTERRUPT, machine_check, 0);
    init_idt_desc(19, GATE_INTERRUPT, simd_floating_point_exception, 0);
    init_idt_desc(20, GATE_INTERRUPT, virtualization_exception, 0);
    init_idt_desc(21, GATE_INTERRUPT, control_protection_exception, 0);
    init_idt_desc(22, GATE_INTERRUPT, reserved2_excepetion, 0);
    init_idt_desc(23, GATE_INTERRUPT, reserved3_excepetion, 0);
    init_idt_desc(24, GATE_INTERRUPT, reserved4_excepetion, 0);
    init_idt_desc(25, GATE_INTERRUPT, reserved5_excepetion, 0);
    init_idt_desc(26, GATE_INTERRUPT, reserved6_excepetion, 0);
    init_idt_desc(27, GATE_INTERRUPT, reserved7_excepetion, 0);
    init_idt_desc(28, GATE_INTERRUPT, hypervisor_injection_exception, 0);
    init_idt_desc(29, GATE_INTERRUPT, vmm_communication_exception, 0);
    init_idt_desc(30, GATE_INTERRUPT, security_exception, 0);
    init_idt_desc(31, GATE_INTERRUPT, reserved8_excepetion, 0);

    //主片IRQ
    init_idt_desc(START_VECTOR + 0, GATE_INTERRUPT, irq0_handler, 0);
    init_idt_desc(START_VECTOR + 1, GATE_INTERRUPT, irq1_handler, 0);
    init_idt_desc(START_VECTOR + 2, GATE_INTERRUPT, irq2_handler, 0);
    init_idt_desc(START_VECTOR + 3, GATE_INTERRUPT, irq3_handler, 0);
    init_idt_desc(START_VECTOR + 4, GATE_INTERRUPT, irq4_handler, 0);
    init_idt_desc(START_VECTOR + 5, GATE_INTERRUPT, irq5_handler, 0);
    init_idt_desc(START_VECTOR + 6, GATE_INTERRUPT, irq6_handler, 0);
    init_idt_desc(START_VECTOR + 7, GATE_INTERRUPT, irq7_handler, 0);

    //从片IRQ
    init_idt_desc(START_VECTOR + 8, GATE_INTERRUPT, irq8_handler, 0);
    init_idt_desc(START_VECTOR + 9, GATE_INTERRUPT, irq9_handler, 0);
    init_idt_desc(START_VECTOR + 10, GATE_INTERRUPT, irq10_handler, 0);
    init_idt_desc(START_VECTOR + 11, GATE_INTERRUPT, irq11_handler, 0);
    init_idt_desc(START_VECTOR + 12, GATE_INTERRUPT, irq12_handler, 0);
    init_idt_desc(START_VECTOR + 13, GATE_INTERRUPT, irq13_handler, 0);
    init_idt_desc(START_VECTOR + 14, GATE_INTERRUPT, irq14_handler, 0);
    init_idt_desc(START_VECTOR + 15, GATE_INTERRUPT, irq15_handler, 0);
}

PUBLIC void init_idt(void) {
    //初始化IDT
    idtr.address = IDT;
    idtr.size = sizeof(IDT) - 1;
    //初始化描述符
    _int_idt_descs();
    //加载IDT
    asmv ("lidtl %0" : : "m"(idtr));
}