; SPDX-License-Identifier: GPL-2.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; 作者: Flysong
;
; arch/x86_64/tayboot/stage3/_int_handlers.asm
;
; 中断处理函数
;



[SECTION .text]
[BITS 32]

;void exception_handler(int vector_no, int errno, int eip, int cs, int eflags);
extern exception_handler

global divide_by_zero_error
global single_step_debug
global non_maskable_interrup
global breakpoint
global overflow
global bound_range_exceeded
global invalid_opcode
global device_not_available
global double_fault
global coprocessor_segment_overrun
global invalid_tss
global segment_not_present
global stack_segment_fault
global general_protection_fault
global page_fault
global reserved1_excepetion
global x87_floating_point_exception	
global alignment_check
global machine_check
global simd_floating_point_exception	
global virtualization_exception
global control_protection_exception
global reserved2_excepetion
global reserved3_excepetion
global reserved4_excepetion
global reserved5_excepetion
global reserved6_excepetion
global reserved7_excepetion
global hypervisor_injection_exception
global vmm_communication_exception
global security_exception
global reserved8_excepetion

%macro excepetion 1
    mov eax, %1
    jmp _exception
%endmacro

divide_by_zero_error:
    push 0xFFFFFFFF
    excepetion 0

single_step_debug:
    push 0xFFFFFFFF
    excepetion 1

non_maskable_interrup:
    push 0xFFFFFFFF
    excepetion 2

breakpoint:
    push 0xFFFFFFFF
    excepetion 3

overflow:
    push 0xFFFFFFFF
    excepetion 4

bound_range_exceeded:
    push 0xFFFFFFFF
    excepetion 5

invalid_opcode:
    push 0xFFFFFFFF
    excepetion 6

device_not_available:
    push 0xFFFFFFFF
    excepetion 7

double_fault:
    excepetion 8

coprocessor_segment_overrun:
    push 0xFFFFFFFF
    excepetion 9

invalid_tss:
    excepetion 10

segment_not_present:
    excepetion 11

stack_segment_fault:
    excepetion 12

general_protection_fault:
    excepetion 13

page_fault:
    excepetion 14

reserved1_excepetion:
    push 0xFFFFFFFF
    excepetion 15

x87_floating_point_exception:
    push 0xFFFFFFFF
    excepetion 16
    
alignment_check:
    excepetion 17

machine_check:
    push 0xFFFFFFFF
    excepetion 18

simd_floating_point_exception:
    push 0xFFFFFFFF
    excepetion 19

virtualization_exception:
    push 0xFFFFFFFF
    excepetion 20

control_protection_exception:
    excepetion 21

reserved2_excepetion:
    push 0xFFFFFFFF
    excepetion 22

reserved3_excepetion:
    push 0xFFFFFFFF
    excepetion 23

reserved4_excepetion:
    push 0xFFFFFFFF
    excepetion 24

reserved5_excepetion:
    push 0xFFFFFFFF
    excepetion 25

reserved6_excepetion:
    push 0xFFFFFFFF
    excepetion 26

reserved7_excepetion:
    push 0xFFFFFFFF
    excepetion 27

hypervisor_injection_exception:
    push 0xFFFFFFFF
    excepetion 28

vmm_communication_exception:
    excepetion 29

security_exception:
    excepetion 30

reserved8_excepetion:
    push 0xFFFFFFFF
    excepetion 31

_exception:
    call exception_handler
    add esp, 4
    hlt
    jmp $

extern irq_interrup_handler

%macro irq_handler 1
    push eax
    mov eax, %1
    call irq_interrup_handler
    pop eax
    iretd
%endmacro

global irq0_handler
global irq1_handler
global irq2_handler
global irq3_handler
global irq4_handler
global irq5_handler
global irq6_handler
global irq7_handler
global irq8_handler
global irq9_handler
global irq10_handler
global irq11_handler
global irq12_handler
global irq13_handler
global irq14_handler
global irq15_handler

irq0_handler:
    irq_handler 0

irq1_handler:
    irq_handler 1

irq2_handler:
    irq_handler 2

irq3_handler:
    irq_handler 3

irq4_handler:
    irq_handler 4

irq5_handler:
    irq_handler 5

irq6_handler:
    irq_handler 6

irq7_handler:
    irq_handler 7

irq8_handler:
    irq_handler 8

irq9_handler:
    irq_handler 9

irq10_handler:
    irq_handler 10

irq11_handler:
    irq_handler 11

irq12_handler:
    irq_handler 12

irq13_handler:
    irq_handler 13

irq14_handler:
    irq_handler 14

irq15_handler:
    irq_handler 15