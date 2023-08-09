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

#include <basec/logger.h>

#include <tay/io.h>
#include <tay/ports.h>

/** IDT */
GateDescriptor IDT[256];
/**
 * @brief IDTR
 * 
 */
DPTR IDTR;

#define IRQ_START (32)

/**
 * @brief 初始化PIC
 * 
 */
void InitPIC(void) {
    outb (M_PIC_BASE + PIC_CONTROL, 0x11);
    outb (S_PIC_BASE + PIC_CONTROL, 0x11); //ICW1

    // IRQ 0
    outb (M_PIC_BASE + PIC_DATA, IRQ_START);
    // IRQ 8
    outb (S_PIC_BASE + PIC_DATA, IRQ_START + 8); //ICW2

    outb (M_PIC_BASE + PIC_DATA, 0x4);
    outb (S_PIC_BASE + PIC_DATA, 0x2); //ICW3

    outb (M_PIC_BASE + PIC_DATA, 0x1);
    outb (S_PIC_BASE + PIC_DATA, 0x1); //ICW4

    // disable all
    outb (M_PIC_BASE + PIC_DATA, 0xFF); //OCW1
    outb (S_PIC_BASE + PIC_DATA, 0xFF);
}

/**
 * @brief 初始化IDT
 * 
 */
void InitIDT(void) {
    IDT[0x00] = BuildGate(GTYPE_386_INT_GATE, DivideByZeroFaultHandler, 0, rdcs());
    IDT[0x01] = BuildGate(GTYPE_386_INT_GATE, SingleStepTrapHandler, 0, rdcs());
    IDT[0x02] = BuildGate(GTYPE_386_INT_GATE, NMIHandler, 0, rdcs());
    IDT[0x03] = BuildGate(GTYPE_386_INT_GATE, BreakpointTrapHandler, 0, rdcs());
    IDT[0x04] = BuildGate(GTYPE_386_INT_GATE, OverflowTrapHandler, 0, rdcs());
    IDT[0x05] = BuildGate(GTYPE_386_INT_GATE, BoundRangeExceededFaultHandler, 0, rdcs());
    IDT[0x06] = BuildGate(GTYPE_386_INT_GATE, InvalidOpcodeFaultHandler, 0, rdcs());
    IDT[0x07] = BuildGate(GTYPE_386_INT_GATE, DeviceNotAvailableFaultHandler, 0, rdcs());
    IDT[0x08] = BuildGate(GTYPE_386_INT_GATE, DoubleFaultHandler, 0, rdcs());
    IDT[0x09] = BuildGate(GTYPE_386_INT_GATE, CoprocessorSegmentOverrunFaultHandler, 0, rdcs());
    IDT[0x0A] = BuildGate(GTYPE_386_INT_GATE, InvalidTSSFault, 0, rdcs());
    IDT[0x0B] = BuildGate(GTYPE_386_INT_GATE, SegmentNotPresentFaultHandler, 0, rdcs());
    IDT[0x0C] = BuildGate(GTYPE_386_INT_GATE, StackSegmentFaultHandler, 0, rdcs());
    IDT[0x0D] = BuildGate(GTYPE_386_INT_GATE, GeneralProtectionFaultHandler, 0, rdcs());
    IDT[0x0E] = BuildGate(GTYPE_386_INT_GATE, PageFaultHandler, 0, rdcs());
    IDT[0x0F] = BuildGate(GTYPE_386_INT_GATE, ReservedHandler1, 0, rdcs());
    IDT[0x10] = BuildGate(GTYPE_386_INT_GATE, X87FloatingPointFaultHandler, 0, rdcs());
    IDT[0x11] = BuildGate(GTYPE_386_INT_GATE, AlignmentCheckHandler, 0, rdcs());
    IDT[0x12] = BuildGate(GTYPE_386_INT_GATE, MachineCheckHandler, 0, rdcs());
    IDT[0x13] = BuildGate(GTYPE_386_INT_GATE, SIMDFloatingPointFaultHandler, 0, rdcs());
    IDT[0x14] = BuildGate(GTYPE_386_INT_GATE, VirtualizationFaultHandler, 0, rdcs());
    IDT[0x15] = BuildGate(GTYPE_386_INT_GATE, ControlProtectionFaultHandler, 0, rdcs());
    IDT[0x16] = BuildGate(GTYPE_386_INT_GATE, ReservedHandler2, 0, rdcs());
    IDT[0x17] = BuildGate(GTYPE_386_INT_GATE, ReservedHandler3, 0, rdcs());
    IDT[0x18] = BuildGate(GTYPE_386_INT_GATE, ReservedHandler4, 0, rdcs());
    IDT[0x19] = BuildGate(GTYPE_386_INT_GATE, ReservedHandler5, 0, rdcs());
    IDT[0x1A] = BuildGate(GTYPE_386_INT_GATE, ReservedHandler6, 0, rdcs());
    IDT[0x1B] = BuildGate(GTYPE_386_INT_GATE, ReservedHandler7, 0, rdcs());
    IDT[0x1C] = BuildGate(GTYPE_386_INT_GATE, HypervisorInjectionException, 0, rdcs());
    IDT[0x1D] = BuildGate(GTYPE_386_INT_GATE, VMMCommunicationFaultHandler, 0, rdcs());
    IDT[0x1E] = BuildGate(GTYPE_386_INT_GATE, SecurityFaultHandler, 0, rdcs());
    IDT[0x1F] = BuildGate(GTYPE_386_INT_GATE, ReservedHandler8, 0, rdcs());

    IDT[IRQ_START + 0] = BuildGate(GTYPE_386_INT_GATE, Irq0Handler, 0, rdcs());
    IDT[IRQ_START + 1] = BuildGate(GTYPE_386_INT_GATE, Irq1Handler, 0, rdcs());
    IDT[IRQ_START + 2] = BuildGate(GTYPE_386_INT_GATE, Irq2Handler, 0, rdcs());
    IDT[IRQ_START + 3] = BuildGate(GTYPE_386_INT_GATE, Irq3Handler, 0, rdcs());
    IDT[IRQ_START + 4] = BuildGate(GTYPE_386_INT_GATE, Irq4Handler, 0, rdcs());
    IDT[IRQ_START + 5] = BuildGate(GTYPE_386_INT_GATE, Irq5Handler, 0, rdcs());
    IDT[IRQ_START + 6] = BuildGate(GTYPE_386_INT_GATE, Irq6Handler, 0, rdcs());
    IDT[IRQ_START + 7] = BuildGate(GTYPE_386_INT_GATE, Irq7Handler, 0, rdcs());
    IDT[IRQ_START + 8] = BuildGate(GTYPE_386_INT_GATE, Irq8Handler, 0, rdcs());
    IDT[IRQ_START + 9] = BuildGate(GTYPE_386_INT_GATE, Irq9Handler, 0, rdcs());
    IDT[IRQ_START + 10] = BuildGate(GTYPE_386_INT_GATE, Irq10Handler, 0, rdcs());
    IDT[IRQ_START + 11] = BuildGate(GTYPE_386_INT_GATE, Irq11Handler, 0, rdcs());
    IDT[IRQ_START + 12] = BuildGate(GTYPE_386_INT_GATE, Irq12Handler, 0, rdcs());
    IDT[IRQ_START + 13] = BuildGate(GTYPE_386_INT_GATE, Irq13Handler, 0, rdcs());
    IDT[IRQ_START + 14] = BuildGate(GTYPE_386_INT_GATE, Irq14Handler, 0, rdcs());
    IDT[IRQ_START + 15] = BuildGate(GTYPE_386_INT_GATE, Irq15Handler, 0, rdcs());
    
    IDTR.address = IDT;
    IDTR.size = sizeof(IDT);

    asm volatile ("lidt %0" : : "m"(IDTR));
}