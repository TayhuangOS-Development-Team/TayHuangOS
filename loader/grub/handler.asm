;
; SPDX-License-Identifier: LGPL-2.1-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; 作者: theflysong
;
; handlers.asm
;
; 中断处理函数
;



[SECTION .text]
[BITS 32]

;void exception_handler(int vector_no, int errno, int eip, int cs, int eflags);
extern exception_handler

;异常列表
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

;无错误码处理器
%macro nonerrcode_handler 1
    push 0xFFFFFFFF
    mov eax, %1
    jmp _exception
%endmacro

%macro errcode_handler 1
    mov eax, %1
    jmp _exception
%endmacro

divide_by_zero_error:               nonerrcode_handler 0
single_step_debug:                  nonerrcode_handler 1
non_maskable_interrup:              nonerrcode_handler 2
breakpoint:                         nonerrcode_handler 3
overflow:                           nonerrcode_handler 4
bound_range_exceeded:               nonerrcode_handler 5
invalid_opcode:                     nonerrcode_handler 6
device_not_available:               nonerrcode_handler 7
double_fault:                          errcode_handler 8
coprocessor_segment_overrun:        nonerrcode_handler 9
invalid_tss:                           errcode_handler 10
segment_not_present:                   errcode_handler 11
stack_segment_fault:                   errcode_handler 12
general_protection_fault:              errcode_handler 13
page_fault:                            errcode_handler 14
reserved1_excepetion:               nonerrcode_handler 15
x87_floating_point_exception:       nonerrcode_handler 16
alignment_check:                       errcode_handler 17
machine_check:                      nonerrcode_handler 18
simd_floating_point_exception:      nonerrcode_handler 19
virtualization_exception:           nonerrcode_handler 20
control_protection_exception:          errcode_handler 21
reserved2_excepetion:               nonerrcode_handler 22
reserved3_excepetion:               nonerrcode_handler 23
reserved4_excepetion:               nonerrcode_handler 24
reserved5_excepetion:               nonerrcode_handler 25
reserved6_excepetion:               nonerrcode_handler 26
reserved7_excepetion:               nonerrcode_handler 27
hypervisor_injection_exception:     nonerrcode_handler 28
vmm_communication_exception:           errcode_handler 29
security_exception:                    errcode_handler 30
reserved8_excepetion:               nonerrcode_handler 31

_exception:
    call exception_handler
    add esp, 4
_hlt:
    hlt
    jmp _hlt
    
extern irq_interrup_handler

;IRQ处理器宏
%macro irq_handler 1
    pushad
    mov eax, %1
    call irq_interrup_handler
    popad
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

irq0_handler:   irq_handler 0
irq1_handler:   irq_handler 1
irq2_handler:   irq_handler 2
irq3_handler:   irq_handler 3
irq4_handler:   irq_handler 4
irq5_handler:   irq_handler 5
irq6_handler:   irq_handler 6
irq7_handler:   irq_handler 7
irq8_handler:   irq_handler 8
irq9_handler:   irq_handler 9
irq10_handler:  irq_handler 10
irq11_handler:  irq_handler 11
irq12_handler:  irq_handler 12
irq13_handler:  irq_handler 13
irq14_handler:  irq_handler 14
irq15_handler:  irq_handler 15
