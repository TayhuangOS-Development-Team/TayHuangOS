/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong Yoyoooo
 *
 * string.h
 *
 * 标准库string.h的头文件
 *
 */



#pragma once

#include <stddef.h>

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

//一些关于字符、字符串、内存的函数:str的
static inline char* strchr(const char* s, char c) {
	register char* __res;
	__asm__("cld\n\t"
		"movb %%al,%%ah\n"
		"1:\tlodsb\n\t"
		"cmpb %%ah,%%al\n\t"
		"je 2f\n\t"
		"testb %%al,%%al\n\t"
		"jne 1b\n\t"
		"movl $1,%1\n"
		"2:\tmovl %1,%0\n\t"
		"decl %0"
		:"=a" (__res) : "S" (s), "0" (c)
	);
	return __res;
}

static inline int strncmp(const char *cs,const char *ct, int count){
    register int __res;
    __asm__("cld\n"
    "1:\tdecl %3\n\t"
    "js 2f\n\t"
    "lodsb\n\t"
    "scasb\n\t"
    "jne  3f\n\t"
    "testb %%al,%%al\n\t"
    "jne 1b\n"
    "2:\txorl %%eax,%%eax\n\t"
    "jmp 4f\n"
    "3:\tmovl $1,%%eax\n\t"
    "jl 4f\n\t"
    "negl %%eax\n"
    "4:"
    :"=a" (__res) : "D" (cs), "S" (ct),"C" (count)  
    );
    return __res;
}

static inline char *strrchr(const char *s,char c){
    register char* __res;
	__asm__("cld\n\t"
		"movb %%al,%%ah\n"
		"1:\tlodsb\n\t"
		"cmpb %%ah,%%al\n\t"
		"jne 2f\n\t"
		"movl %%esi,%0\n\t"
		"decl %0\n"
		"2:\ttestb %%al,%%al\n\t"
		"jne 1b"
		:"=d" (__res) : "0" (0), "S" (s), "a" (c)
	);
	return __res;
}

static inline int strspn(const char* cs, const char* ct) {
	register char* __res;
	__asm__("cld\n\t"
		"movl %4,%%edi\n\t"
		"repne\n\t"
		"scasb\n\t"
		"notl %%ecx\n\t"
		"decl %%ecx\n\t"
		"movl %%ecx,%%edx\n"
		"1:\tlodsb\n\t"
		"testb %%al,%%al\n\t"
		"je 2f\n\t"
		"movl %4,%%edi\n\t"
		"movl %%edx,%%ecx\n\t"
		"repne\n\t"
		"scasb\n\t"
		"je 1b\n"
		"2:\tdecl %0"
		:"=S" (__res) : "a" (0), "c" (0xffffffff), "0" (cs), "g" (ct)
	);
	return __res - cs;
}

static inline int strcspn(const char* cs, const char* ct) {
	register char* __res;
	__asm__("cld\n\t"
		"movl %4,%%edi\n\t"
		"repne\n\t"
		"scasb\n\t"
		"notl %%ecx\n\t"
		"decl %%ecx\n\t"
		"movl %%ecx,%%edx\n"
		"1:\tlodsb\n\t"
		"testb %%al,%%al\n\t"
		"je 2f\n\t"
		"movl %4,%%edi\n\t"
		"movl %%edx,%%ecx\n\t"
		"repne\n\t"
		"scasb\n\t"
		"jne 1b\n"
		"2:\tdecl %0"
		:"=S" (__res) : "a" (0), "c" (0xffffffff), "0" (cs), "g" (ct)
	);
	return __res - cs;
}

static inline char* strpbrk(const char* cs, const char* ct) {
	register char* __res;
	__asm__("cld\n\t"
		"movl %4,%%edi\n\t"
		"repne\n\t"
		"scasb\n\t"
		"notl %%ecx\n\t"
		"decl %%ecx\n\t"
		"movl %%ecx,%%edx\n"
		"1:\tlodsb\n\t"
		"testb %%al,%%al\n\t"
		"je 2f\n\t"
		"movl %4,%%edi\n\t"
		"movl %%edx,%%ecx\n\t"
		"repne\n\t"
		"scasb\n\t"
		"jne 1b\n\t"
		"decl %0\n\t"
		"jmp 3f\n"
		"2:\txorl %0,%0\n"
		"3:"
		:"=S" (__res) : "a" (0), "c" (0xffffffff), "0" (cs), "g" (ct)
	);
	return __res;
}

static inline char* strncpy(char* dest, const char* src, int count) {
	__asm__("cld\n"
		"1:\tdecl %2\n\t"
		"js 2f\n\t"
		"lodsb\n\t"
		"stosb\n\t"
		"testb %%al,%%al\n\t"
		"jne 1b\n\t"
		"rep\n\t"
		"stosb\n"
		"2:"
		::"S" (src), "D" (dest), "c" (count)
	);
	return dest;
}

static inline char* strcat(char* dest, const char* src) {
	__asm__("cld\n\t"
		"repne\n\t"
		"scasb\n\t"
		"decl %1\n"
		"1:\tlodsb\n\t"
		"stosb\n\t"
		"testb %%al,%%al\n\t"
		"jne 1b"
		::"S" (src), "D" (dest), "a" (0), "c" (0xffffffff)
	);
	return dest;
}

static inline char* strncat(char* dest, const char* src, int count) {
	__asm__("cld\n\t"
		"repne\n\t"
		"scasb\n\t"
		"decl %1\n\t"
		"movl %4,%3\n"
		"1:\tdecl %3\n\t"
		"js 2f\n\t"
		"lodsb\n\t"
		"stosb\n\t"
		"testb %%al,%%al\n\t"
		"jne 1b\n"
		"2:\txorl %2,%2\n\t"
		"stosb"
		::"S" (src), "D" (dest), "a" (0), "c" (0xffffffff), "g" (count)
	);
	return dest;
}

static inline char* strtok(char* s, const char* ct) {
	char* ___strtok;

	register char* __res;
	__asm__("testl %1,%1\n\t"
		"jne 1f\n\t"
		"testl %0,%0\n\t"
		"je 8f\n\t"
		"movl %0,%1\n"
		"1:\txorl %0,%0\n\t"
		"movl $-1,%%ecx\n\t"
		"xorl %%eax,%%eax\n\t"
		"cld\n\t"
		"movl %4,%%edi\n\t"
		"repne\n\t"
		"scasb\n\t"
		"notl %%ecx\n\t"
		"decl %%ecx\n\t"
		"je 7f\n\t"			/* empty delimeter-string */
		"movl %%ecx,%%edx\n"
		"2:\tlodsb\n\t"
		"testb %%al,%%al\n\t"
		"je 7f\n\t"
		"movl %4,%%edi\n\t"
		"movl %%edx,%%ecx\n\t"
		"repne\n\t"
		"scasb\n\t"
		"je 2b\n\t"
		"decl %1\n\t"
		"cmpb $0,(%1)\n\t"
		"je 7f\n\t"
		"movl %1,%0\n"
		"3:\tlodsb\n\t"
		"testb %%al,%%al\n\t"
		"je 5f\n\t"
		"movl %4,%%edi\n\t"
		"movl %%edx,%%ecx\n\t"
		"repne\n\t"
		"scasb\n\t"
		"jne 3b\n\t"
		"decl %1\n\t"
		"cmpb $0,(%1)\n\t"
		"je 5f\n\t"
		"movb $0,(%1)\n\t"
		"incl %1\n\t"
		"jmp 6f\n"
		"5:\txorl %1,%1\n"
		"6:\tcmpb $0,(%0)\n\t"
		"jne 7f\n\t"
		"xorl %0,%0\n"
		"7:\ttestl %0,%0\n\t"
		"jne 8f\n\t"
		"movl %0,%1\n"
		"8:"
		:"=b" (__res), "=S" (___strtok)
		: "0" (___strtok), "1" (s), "g" (ct)
	);
	return __res;
}

static inline void* memmove(void* dest, const void* src, int n) {
	if (dest < src)
		__asm__("cld\n\t"
			"rep\n\t"
			"movsb"
			::"c" (n), "S" (src), "D" (dest)
		);
	else
		__asm__("std\n\t"
			"rep\n\t"
			"movsb"
			::"c" (n), "S" (src + n - 1), "D" (dest + n - 1)
		);
	return dest;
}

static inline int memcmp(const void* cs, const void* ct, int count) {
	register int __res;
	__asm__("cld\n\t"
		"repe\n\t"
		"cmpsb\n\t"
		"je 1f\n\t"
		"movl $1,%%eax\n\t"
		"jl 1f\n\t"
		"negl %%eax\n"
		"1:"
		:"=a" (__res) : "0" (0), "D" (cs), "S" (ct), "c" (count)
	);
	return __res;
}

static inline void* memchr(const void* cs, char c, int count) {
	register void* __res;
	if (!count)
		return NULL;
	__asm__("cld\n\t"
		"repne\n\t"
		"scasb\n\t"
		"je 1f\n\t"
		"movl $1,%0\n"
		"1:\tdecl %0"
		:"=D" (__res) : "a" (c), "D" (cs), "c" (count)
	);
	return __res;
}

#endif