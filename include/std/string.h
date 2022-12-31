/* 
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong Yoyoooo
 *
 * string.h
 *
 * 标准库string.h的头文件
 *
 */



#pragma once

#include <stddef.h>

// memset
static inline void memset(void *dst, int val, size_t sz) {
    unsigned long long _val = val;
    __asm__ __volatile__ ("\
cld\n\
rep\n\
stosb" : : "c"(sz), "a"(_val), "D"(dst));
}

// memcpy
static inline void memcpy(void *dst, const void *src, size_t sz) {
    __asm__ __volatile__ ("\
cld\n\
rep\n\
movsb" : : "c"(sz), "S"(src), "D"(dst));
}

// 比较字符串
static inline int strcmp(const char *str1, const char *str2) {
	int ret = 0;
	do {
		ret = *(str1) - *(str2);
	} while (*str1 != 0 && *str2 != 0 && ret == 0);
	if (ret < 0) return -1;
	else if (ret > 0) return 1;
	return 0;
}

//计算str长度
static inline int strlen(const char *str) {
	int cnt = 0x7FFFFFFF;
    //使用字符串扫描指令
    __asm__ ("\
	cld\n\
    repnz\n\
    scasb" : "+c" (cnt) : "D" (str), "a" (0));
    return 0x7FFFFFFF - cnt - 1;
} 

// 复制字符串
static inline char *strcpy(void *dst, const char *src) {
	char *_dst = (char *)dst;
	while (*src != 0) {
		*_dst = *src;
		_dst ++;
		src ++;
	}
	return dst;
}

// 查找字符
static inline char *strchr(const char *str, char ch) {
	while (*str != 0) {
		if (*str == ch) {
			return str;
		}
	}
	return NULL;
}

// 匹配字符串(前count个字符)
static inline int strncmp(const char *str1, const char *str2, int count){
	int ret = 0;
	do {
		ret = *(str1) - *(str2);
		count --;
	} while (*str1 != 0 && *str2 != 0 && ret == 0 && count > 0);
	if (ret < 0) return -1;
	else if (ret > 0) return 1;
	return 0;
}

// 复制字符串(前count个字符)
static inline char *strncpy(char *dst, const char *src, int count) {
	char *_dst = (char *)dst;
	while (*src != 0 && count > 0) {
		*_dst = *src;
		_dst ++;
		src ++;
		count --;
	}
	return dst;
}

// 查找字符(末次出现)
static inline char *strrchr(const char *str, char ch){
	char *ret = NULL;
	while (*str != 0) {
		if (*str == ch) {
			ret = str;
		}
	}
	return ret;
}

// 返回str从开头开始连续在accept中出现过的字符的个数
static inline int strspn(const char *str, const char *accept) {
	char map[128] = {};
	while (*accept != 0) {
		map[*accept] = 1;
		accept ++;
	}
	int cnt = 0;
	while (*str != 0) {
		if (! map[*str]) {
			break;
		}
		cnt ++;
	}
	return cnt;
}

// 返回str从开头开始连续没在accept中出现过的字符的个数
static inline int strcspn(const char *str, const char *accept) {
	char map[128] = {};
	while (*accept != 0) {
		map[*accept] = 1;
		accept ++;
	}
	int cnt = 0;
	while (*str != 0) {
		if (map[*str]) {
			break;
		}
		cnt ++;
	}
	return cnt;
}

// 返回str在accept中出现过的第一个字符的指针
static inline char *strpbrk(const char *str, const char *accept) {
	return str + strcspn(str, accept);
}

// 将src拼接在dst上
static inline char *strcat(char *dst, const char *src) {
	char *cat_pos = dst + strlen(dst);
	while (*src != 0) {
		*cat_pos = *src;
		cat_pos ++;
		src ++;
	}
	return dst;
}

// 将src的前count个字符拼接在dst上
static inline char *strncat(char *dst, const char *src, int count) {
	char *cat_pos = dst + strlen(dst);
	while (*src != 0 && count > 0) {
		*cat_pos = *src;
		cat_pos ++;
		src ++;
		count --;
	}
	return dst;
}

// 分割字符串
// TMD这玩意谁tmd爱写谁tmd写去
static inline char *strtok(char *str, const char *split) {
	return NULL;
}

// 从src中复制size个字节到dst中
static inline void *memmove(void *dst, const void *src, int size) {
	if (dst < src) {
		__asm__("\
	cld\n\
	rep\n\
	movsb" :: "c"(size), "S"(src), "D"(dst)
		);
	}
	else {
		__asm__("\
	std\n\
	rep\n\
	movsb" :: "c"(size), "S"(src + size - 1), "D"(dst + size - 1)
		);
	}
	return dst;
}

// 比较内存中前count个字节
static inline int memcmp(const void *str1, const void *str2, int count) {
	int ret = 0;
	const char *_str1 = str1, *_str2 = str2;
	do {
		ret = *(_str1) - *(_str2);
		count --;
	} while (ret == 0 && count > 0);
	if (ret < 0) return -1;
	else if (ret > 0) return 1;
	return 0;
}

// 在内存的前count个字节中查找ch
static inline void *memchr(const void *str, char ch, int count) {
	const char *_str = str;
	while (count > 0) {
		if (*_str == ch) {
			return _str;
		}
		count --;
	}
	return NULL;
}