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
 * kheap.h
 *
 * 内核堆
 *
 */



#pragma once

#include <tayhuang/defs.h>

#define KHEAP_BASE (0x100000)

static inline void memset(_OUT void *dst, _IN byte val, _IN qword sz) {
    qword _val = val;
    asmv ("movq %0, %%rcx\n\t\
movq %1, %%rax\n\t\
movq %2, %%rdi\n\t\
cld\n\t\
rep\n\t\
stosb" : : "g"(sz), "g"(_val), "g"(dst) : "%rcx", "%rdi", "%rax");
}

static inline void memcpy(_OUT void *dst, _IN void *src, _IN qword sz) {
    asmv ("movq %0, %%rcx\n\t\
movq %1, %%rsi\n\t\
movq %2, %%rdi\n\t\
cld\n\t\
rep\n\t\
movsb" : : "g"(sz), "g"(src), "g"(dst) : "%rcx", "%rdi", "%rsi");
}

PUBLIC void init_kheap(_IN void *kheap_limit);
PUBLIC void *malloc(_IN int size);
static inline void *calloc(_IN int num, _IN int size) {
    return malloc(num * size);
}
PUBLIC void free(_IN void *ptr);