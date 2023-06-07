/**
 * @file idt.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief IDT
 * @version alpha-1.0.0
 * @date 2023-05-01
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <init/idt.h>
#include <init/handler.h>

#include <primlib/logger.h>

#include <tay/io.h>
#include <tay/ports.h>

gate_desc_t IDT[256];
dptr_t IDTR;

void init_pic(void) {
    outb (M_PIC_CONTROL, 0x11);
    outb (S_PIC_CONTROL, 0x11); //ICW1

    outb (M_PIC_DATA, 0);
    outb (S_PIC_DATA, 8); //ICW2

    outb (M_PIC_DATA, 0x4);
    outb (S_PIC_DATA, 0x2); //ICW3

    outb (M_PIC_DATA, 0x1);
    outb (S_PIC_DATA, 0x1); //ICW4

    outb (M_PIC_DATA, 0xFF); //OCW1
    outb (S_PIC_DATA, 0xFF);
}

void init_idt(void) {
    IDT[0x00] = build_idt(GTYPE_386_INT_GATE, divide_by_zero_error, 0, rdcs());
    IDT[0x01] = build_idt(GTYPE_386_INT_GATE, single_step_debug, 0, rdcs());
    IDT[0x02] = build_idt(GTYPE_386_INT_GATE, non_maskable_interrup, 0, rdcs());
    IDT[0x03] = build_idt(GTYPE_386_INT_GATE, breakpoint, 0, rdcs());
    IDT[0x04] = build_idt(GTYPE_386_INT_GATE, overflow, 0, rdcs());
    IDT[0x05] = build_idt(GTYPE_386_INT_GATE, bound_range_exceeded, 0, rdcs());
    IDT[0x06] = build_idt(GTYPE_386_INT_GATE, invalid_opcode, 0, rdcs());
    IDT[0x07] = build_idt(GTYPE_386_INT_GATE, device_not_available, 0, rdcs());
    IDT[0x08] = build_idt(GTYPE_386_INT_GATE, double_fault, 0, rdcs());
    IDT[0x09] = build_idt(GTYPE_386_INT_GATE, coprocessor_segment_overrun, 0, rdcs());
    IDT[0x0A] = build_idt(GTYPE_386_INT_GATE, invalid_tss, 0, rdcs());
    IDT[0x0B] = build_idt(GTYPE_386_INT_GATE, segment_not_present, 0, rdcs());
    IDT[0x0C] = build_idt(GTYPE_386_INT_GATE, stack_segment_fault, 0, rdcs());
    IDT[0x0D] = build_idt(GTYPE_386_INT_GATE, general_protection_fault, 0, rdcs());
    IDT[0x0E] = build_idt(GTYPE_386_INT_GATE, page_fault, 0, rdcs());
    IDT[0x0F] = build_idt(GTYPE_386_INT_GATE, reserved1_excepetion, 0, rdcs());
    IDT[0x10] = build_idt(GTYPE_386_INT_GATE, x87_floating_point_exception, 0, rdcs());
    IDT[0x11] = build_idt(GTYPE_386_INT_GATE, alignment_check, 0, rdcs());
    IDT[0x12] = build_idt(GTYPE_386_INT_GATE, machine_check, 0, rdcs());
    IDT[0x13] = build_idt(GTYPE_386_INT_GATE, simd_floating_point_exception, 0, rdcs());
    IDT[0x14] = build_idt(GTYPE_386_INT_GATE, virtualization_exception, 0, rdcs());
    IDT[0x15] = build_idt(GTYPE_386_INT_GATE, control_protection_exception, 0, rdcs());
    IDT[0x16] = build_idt(GTYPE_386_INT_GATE, reserved2_excepetion, 0, rdcs());
    IDT[0x17] = build_idt(GTYPE_386_INT_GATE, reserved3_excepetion, 0, rdcs());
    IDT[0x18] = build_idt(GTYPE_386_INT_GATE, reserved4_excepetion, 0, rdcs());
    IDT[0x19] = build_idt(GTYPE_386_INT_GATE, reserved5_excepetion, 0, rdcs());
    IDT[0x1A] = build_idt(GTYPE_386_INT_GATE, reserved6_excepetion, 0, rdcs());
    IDT[0x1B] = build_idt(GTYPE_386_INT_GATE, reserved7_excepetion, 0, rdcs());
    IDT[0x1C] = build_idt(GTYPE_386_INT_GATE, hypervisor_injection_exception, 0, rdcs());
    IDT[0x1D] = build_idt(GTYPE_386_INT_GATE, vmm_communication_exception, 0, rdcs());
    IDT[0x1E] = build_idt(GTYPE_386_INT_GATE, security_exception, 0, rdcs());
    IDT[0x1F] = build_idt(GTYPE_386_INT_GATE, reserved8_excepetion, 0, rdcs());
    
    IDTR.address = IDT;
    IDTR.size = sizeof(IDT);

    asm volatile ("lidt %0" : : "m"(IDTR));
}