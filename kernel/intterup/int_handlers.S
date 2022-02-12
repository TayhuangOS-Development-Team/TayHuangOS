/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: Flysong
 * 
 * int_handlers.S
 * 
 * 中断处理
 * 
 */

.extern general_irq_handler
.extern general_exception_handler

.macro irq_handler irq
    pushq %rdi
    movq $\irq, %rdi
    call general_irq_handler
    popq %rax
    iret
.endm

.macro nonerrcode_exception vector
    pushq %rsi
    pushq %rdi
    pushq %rcx
    pushq %rdx
    pushq %r8
    movq $\vector, %rdi
    movq $0xFFFFFFFF, %rsi
    movq 40(%rsp), %rcx
    movq 48(%rsp), %rdx
    movzx 56(%rsp), %r8
    jmp _exception
.endm

.macro errcode_exception vector
    pushq %rsi
    pushq %rdi
    pushq %rcx
    pushq %rdx
    pushq %r8
    movq $\vector, %rdi
    movq 40(%rsp), %rsi
    movq 48(%rsp), %rcx
    movq 56(%rsp), %rdx
    movzx 64(%rsp), %r8
    jmp _exception
.endm

_exception:
    call general_exception_handler
    popq %r8
    popq %rdx
    popq %rcx
    popq %rdi
    popq %rsi
    iret


.global divide_by_zero_error
.type   divide_by_zero_error, @function
.global single_step_debug
.type   single_step_debug, @function
.global non_maskable_interrup
.type   non_maskable_interrup, @function
.global breakpoint
.type   breakpoint, @function
.global overflow
.type   overflowr, @function
.global bound_range_exceeded
.type   bound_range_exceeded, @function
.global invalid_opcode
.type   invalid_opcode, @function
.global device_not_available
.type   device_not_available, @function
.global double_fault
.type   double_fault, @function
.global coprocessor_segment_overrun
.type   coprocessor_segment_overrun, @function
.global invalid_tss
.type   invalid_tss, @function
.global segment_not_present
.type   segment_not_present, @function
.global stack_segment_fault
.type   stack_segment_fault, @function
.global general_protection_fault
.type   general_protection_fault, @function
.global page_fault
.type   page_fault, @function
.global reserved1_excepetion
.type   reserved1_excepetion, @function
.global x87_floating_point_exception	
.type   x87_floating_point_exception, @function
.global alignment_check
.type   alignment_check, @function
.global machine_check
.type   machine_check, @function
.global simd_floating_point_exception	
.type   simd_floating_point_exception, @function
.global virtualization_exception
.type   virtualization_exception, @function
.global control_protection_exception
.type   control_protection_exception, @function
.global reserved2_excepetion
.type   reserved2_excepetion, @function
.global reserved3_excepetion
.type   reserved3_excepetion, @function
.global reserved4_excepetion
.type   reserved4_excepetion, @function
.global reserved5_excepetion
.type   reserved5_excepetion, @function
.global reserved6_excepetion
.type   reserved6_excepetion, @function
.global reserved7_excepetion
.type   reserved7_excepetion, @function
.global hypervisor_injection_exception
.type   hypervisor_injection_exception, @function
.global vmm_communication_exception
.type   vmm_communication_exception, @function
.global security_exception
.type   security_exception, @function
.global reserved8_excepetion
.type   reserved8_excepetion, @function


divide_by_zero_error:
    nonerrcode_exception 0
single_step_debug:
    nonerrcode_exception 1
non_maskable_interrup:
    nonerrcode_exception 2
breakpoint:
    nonerrcode_exception 3
overflow:
    nonerrcode_exception 4
bound_range_exceeded:
    nonerrcode_exception 5
invalid_opcode:
    nonerrcode_exception 6
device_not_available:
    nonerrcode_exception 7
double_fault:
    errcode_exception 8
coprocessor_segment_overrun:
    nonerrcode_exception 9
invalid_tss:
    errcode_exception 10
segment_not_present:
    errcode_exception 11
stack_segment_fault:
    errcode_exception 12
general_protection_fault:
    errcode_exception 13
page_fault:
    errcode_exception 14
reserved1_excepetion:
    nonerrcode_exception 15
x87_floating_point_exception:
    nonerrcode_exception 16
alignment_check:
    errcode_exception 17
machine_check:
    nonerrcode_exception 18
simd_floating_point_exception:
    nonerrcode_exception 19
virtualization_exception:
    nonerrcode_exception 20
control_protection_exception:
    errcode_exception 21
reserved2_excepetion:
    nonerrcode_exception 22
reserved3_excepetion:
    nonerrcode_exception 23
reserved4_excepetion:
    nonerrcode_exception 24
reserved5_excepetion:
    nonerrcode_exception 25
reserved6_excepetion:
    nonerrcode_exception 26
reserved7_excepetion:
    nonerrcode_exception 27
hypervisor_injection_exception:
    nonerrcode_exception 28
vmm_communication_exception:
    errcode_exception 29
security_exception:
    errcode_exception 30
reserved8_excepetion:
    nonerrcode_exception 31
