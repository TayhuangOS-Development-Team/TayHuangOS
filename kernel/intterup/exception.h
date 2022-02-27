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
 * exception.h
 *
 * 异常处理
 *
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC void general_exception_handler(int vector, int errcode, long long cs, long long rip, word eflags, void *registers);
PUBLIC void divide_by_zero_error(void);
PUBLIC void single_step_debug(void);
PUBLIC void non_maskable_interrup(void);
PUBLIC void breakpoint(void);
PUBLIC void overflow(void);
PUBLIC void bound_range_exceeded(void);
PUBLIC void invalid_opcode(void);
PUBLIC void device_not_available(void);
PUBLIC void double_fault(void);
PUBLIC void coprocessor_segment_overrun(void);
PUBLIC void invalid_tss(void);
PUBLIC void segment_not_present(void);
PUBLIC void stack_segment_fault(void);
PUBLIC void general_protection_fault(void);
PUBLIC void page_fault(void);
PUBLIC void reserved1_excepetion(void);
PUBLIC void x87_floating_point_exception(void);
PUBLIC void alignment_check(void);
PUBLIC void machine_check(void);
PUBLIC void simd_floating_point_exception(void);
PUBLIC void virtualization_exception(void);
PUBLIC void control_protection_exception(void);
PUBLIC void reserved2_excepetion(void);
PUBLIC void reserved3_excepetion(void);
PUBLIC void reserved4_excepetion(void);
PUBLIC void reserved5_excepetion(void);
PUBLIC void reserved6_excepetion(void);
PUBLIC void reserved7_excepetion(void);
PUBLIC void hypervisor_injection_exception(void);
PUBLIC void vmm_communication_exception(void);
PUBLIC void security_exception(void);
PUBLIC void reserved8_excepetion(void);