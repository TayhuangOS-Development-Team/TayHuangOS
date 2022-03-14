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

.macro i_x64_pushad
    pushq %rsp
    pushq %rbp
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    pushfq
.endm

.macro i_x64_popad
    popfq
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
    popq %rbp
    popq %rsp
.endm

.macro irq_handler irq
    i_x64_pushad
    pushq %rdi
    movq $\irq, %rdi
    call general_irq_handler
    popq %rax
    i_x64_popad
    iretq
.endm

.macro nonerrcode_exception vector
    subq $8, %rsp
    movl $0xFFFFFFFF, (%rsp)
    i_x64_pushad
    movq %rsp, %rbp
    addq $136, %rbp
    addq $24, 128(%rsp)
    movq $\vector, %rdi
    movq 0(%rbp), %rsi
    movq 8(%rbp), %rcx
    movq 16(%rbp), %rdx
    movzx 24(%rbp), %r8
    movq %rsp, %r9
    jmp _exception
.endm

.macro errcode_exception vector
    i_x64_pushad
    movq %rsp, %rbp
    addq $136, %rbp
    addq $24, 128(%rsp)
    movq $\vector, %rdi
    movq 0(%rbp), %rsi
    movq 8(%rbp), %rcx
    movq 16(%rbp), %rdx
    movzx 24(%rbp), %r8
    movq %rsp, %r9
    jmp _exception
.endm

_exception:
    call general_exception_handler
    popq %rax
    i_x64_popad
    iretq


.global divide_by_zero_error
.type   divide_by_zero_error, @function
.global single_step_debug
.type   single_step_debug, @function
.global non_maskable_interrup
.type   non_maskable_interrup, @function
.global breakpoint
.type   breakpoint, @function
.global overflow
.type   overflow, @function
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

.global clock_irq_handler
.type   clock_irq_handler, @function
.global irq1_handler
.type   irq1_handler, @function
.global irq2_handler
.type   irq2_handler, @function
.global irq3_handler
.type   irq3_handler, @function
.global irq4_handler
.type   irq4_handler, @function
.global irq5_handler
.type   irq5_handler, @function
.global irq6_handler
.type   irq6_handler, @function
.global irq7_handler
.type   irq7_handler, @function
.global irq8_handler
.type   irq8_handler, @function
.global irq9_handler
.type   irq9_handler, @function
.global irq10_handler
.type   irq10_handler, @function
.global irq11_handler
.type   irq11_handler, @function
.global irq12_handler
.type   irq12_handler, @function
.global irq13_handler
.type   irq13_handler, @function
.global irq14_handler
.type   irq14_handler, @function
.global irq15_handler
.type   irq15_handler, @function

.extern clock_int_handler

clock_irq_handler:
    i_x64_pushad
    movq $0, %rdi
    movq %rsp, %rsi
    call clock_int_handler
    i_x64_popad
    iretq

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
