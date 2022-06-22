/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * string.h
 *
 * 标准库string.h的头文件
 *
 */



#pragma once

int strcmp(const char *str1, const char *str2); //比较两个字符串，相同返回0, str1 > str2 返回 1, str1 < str2 返回-1
int strlen(const char *str); //计算str长度
char *strcpy(void *dst, const char *src); //复制字符串src至字符串dst

#ifndef LOADER32BIT

static inline void memset(void *dst, int val, unsigned long long sz) {
    unsigned long long _val = val;
    __asm__ __volatile__ ("movq %0, %%rcx\n\t\
movq %1, %%rax\n\t\
movq %2, %%rdi\n\t\
cld\n\t\
rep\n\t\
stosb" : : "g"(sz), "g"(_val), "g"(dst) : "%rcx", "%rdi", "%rax");
}

static inline void memcpy(void *dst, void *src, unsigned long long sz) {
    __asm__ __volatile__ ("movq %0, %%rcx\n\t\
movq %1, %%rsi\n\t\
movq %2, %%rdi\n\t\
cld\n\t\
rep\n\t\
movsb" : : "g"(sz), "g"(src), "g"(dst) : "%rcx", "%rdi", "%rsi");
}

#else


static inline void memset(void *dst, int val, unsigned int sz) {
    unsigned int _val = val;
    __asm__ __volatile__ ("movl %0, %%ecx\n\t\
movl %1, %%eax\n\t\
movl %2, %%edi\n\t\
cld\n\t\
rep\n\t\
stosb" : : "g"(sz), "g"(_val), "g"(dst) : "%ecx", "%edi", "%eax");
}

static inline void memcpy(void *dst, void *src, unsigned int sz) {
    __asm__ __volatile__ ("movl %0, %%ecx\n\t\
movl %1, %%esi\n\t\
movl %2, %%edi\n\t\
cld\n\t\
rep\n\t\
movsb" : : "g"(sz), "g"(src), "g"(dst) : "%ecx", "%edi", "%esi");
}

#endif