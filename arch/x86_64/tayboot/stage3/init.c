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
 * init.c
 *
 * 初始化函数
 *
 */



#include "init.h"
#include "int_handlers.h"
#include <tayboot/int_vectors.h>
#include <tayboot/ports.h>

struct desc_struct GDT[GDT_SIZE];
struct gdt_ptr gdtr;
struct gate_struct IDT[IDT_SIZE];
struct desc_ptr idtr;

PUBLIC void init_gdt(void) {
    GDT[0] = (struct desc_struct)GDT_ENTRY(0, 0, 0xFFFFF); //空
    GDT[1] = (struct desc_struct)GDT_ENTRY(0xC09A, 0, 0xFFFFF); //代码段
    GDT[2] = (struct desc_struct)GDT_ENTRY(0xC093, 0, 0xFFFFF); //数据段
    GDT[3] = (struct desc_struct)GDT_ENTRY(0x0089, 4096, 103); //TSS
    GDT[4] = (struct desc_struct)GDT_ENTRY(0xA09A, 0, 0xFFFFF); //代码段64bit
    gdtr.ptr = GDT;
    gdtr.len = sizeof (GDT) - 1;
    asmv ("lgdtl %0" : : "m"(gdtr)); //加载GDT
}

PUBLIC void init_pic(void) {
    outb (M_PIC_CONTROL, 0x11);
    outb (S_PIC_CONTROL, 0x11); //ICW1

    outb (M_PIC_DATA, INT_VECTOR_IRQ0);
    outb (S_PIC_DATA, INT_VECTOR_IRQ8); //ICW2

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
    init_idt_desc(CALC_IRQ(0), GATE_INTERRUPT, irq0_handler, 0);
    init_idt_desc(CALC_IRQ(1), GATE_INTERRUPT, irq1_handler, 0);
    init_idt_desc(CALC_IRQ(2), GATE_INTERRUPT, irq2_handler, 0);
    init_idt_desc(CALC_IRQ(3), GATE_INTERRUPT, irq3_handler, 0);
    init_idt_desc(CALC_IRQ(4), GATE_INTERRUPT, irq4_handler, 0);
    init_idt_desc(CALC_IRQ(5), GATE_INTERRUPT, irq5_handler, 0);
    init_idt_desc(CALC_IRQ(6), GATE_INTERRUPT, irq6_handler, 0);
    init_idt_desc(CALC_IRQ(7), GATE_INTERRUPT, irq7_handler, 0);

    //从片IRQ
    init_idt_desc(CALC_IRQ(8), GATE_INTERRUPT, irq8_handler, 0);
    init_idt_desc(CALC_IRQ(9), GATE_INTERRUPT, irq9_handler, 0);
    init_idt_desc(CALC_IRQ(10), GATE_INTERRUPT, irq10_handler, 0);
    init_idt_desc(CALC_IRQ(11), GATE_INTERRUPT, irq11_handler, 0);
    init_idt_desc(CALC_IRQ(12), GATE_INTERRUPT, irq12_handler, 0);
    init_idt_desc(CALC_IRQ(13), GATE_INTERRUPT, irq13_handler, 0);
    init_idt_desc(CALC_IRQ(14), GATE_INTERRUPT, irq14_handler, 0);
    init_idt_desc(CALC_IRQ(15), GATE_INTERRUPT, irq15_handler, 0);
}

PUBLIC void init_idt(void) {
    //初始化IDT
    idtr.address = IDT;
    idtr.size = sizeof(IDT) - 1;
    _int_idt_descs();
    asmv ("lidtl %0" : : "m"(idtr));
}