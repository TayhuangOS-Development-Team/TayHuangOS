; SPDX-License-Identifier: GPL-2.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; Author: Flysong
;
; kernel/boot/stage3/_int_handlers.asm
;
; Intterup Handlers here
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

divide_by_zero_error:
    push 0xFFFFFFFF
    mov eax, 0
    jmp _exception

single_step_debug:
    push 0xFFFFFFFF
    mov eax, 1
    jmp _exception

non_maskable_interrup:
    push 0xFFFFFFFF
    mov eax, 2
    jmp _exception

breakpoint:
    push 0xFFFFFFFF
    mov eax, 3
    jmp _exception

overflow:
    push 0xFFFFFFFF
    mov eax, 4
    jmp _exception

bound_range_exceeded:
    push 0xFFFFFFFF
    mov eax, 5
    jmp _exception

invalid_opcode:
    push 0xFFFFFFFF
    mov eax, 6
    jmp _exception

device_not_available:
    push 0xFFFFFFFF
    mov eax, 7
    jmp _exception

double_fault:
    mov eax, 8
    jmp _exception

coprocessor_segment_overrun:
    push 0xFFFFFFFF
    mov eax, 9
    jmp _exception

invalid_tss:
    mov eax, 10
    jmp _exception

segment_not_present:
    mov eax, 11
    jmp _exception

stack_segment_fault:
    mov eax, 12
    jmp _exception

general_protection_fault:
    mov eax, 13
    jmp _exception

page_fault:
    mov eax, 14
    jmp _exception

reserved1_excepetion:
    push 0xFFFFFFFF
    mov eax, 15
    jmp _exception

x87_floating_point_exception:
    push 0xFFFFFFFF
    mov eax, 16
    jmp _exception
    
alignment_check:
    mov eax, 17
    jmp _exception

machine_check:
    push 0xFFFFFFFF
    mov eax, 18
    jmp _exception

simd_floating_point_exception:
    push 0xFFFFFFFF
    mov eax, 19
    jmp _exception

virtualization_exception:
    push 0xFFFFFFFF
    mov eax, 20
    jmp _exception

control_protection_exception:
    mov eax, 21
    jmp _exception

reserved2_excepetion:
    push 0xFFFFFFFF
    mov eax, 22
    jmp _exception

reserved3_excepetion:
    push 0xFFFFFFFF
    mov eax, 23
    jmp _exception

reserved4_excepetion:
    push 0xFFFFFFFF
    mov eax, 24
    jmp _exception

reserved5_excepetion:
    push 0xFFFFFFFF
    mov eax, 25
    jmp _exception

reserved6_excepetion:
    push 0xFFFFFFFF
    mov eax, 26
    jmp _exception

reserved7_excepetion:
    push 0xFFFFFFFF
    mov eax, 27
    jmp _exception

hypervisor_injection_exception:
    push 0xFFFFFFFF
    mov eax, 28
    jmp _exception

vmm_communication_exception:
    mov eax, 29
    jmp _exception

security_exception:
    mov eax, 30
    jmp _exception

reserved8_excepetion:
    push 0xFFFFFFFF
    mov eax, 31
    jmp _exception

_exception:
    call exception_handler
    add esp, 4 * 2
    hlt
    jmp $