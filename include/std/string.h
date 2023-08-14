/**
 * @file string.h
 * @author theflysong (song_of_the_fly@163.com) Yoyoooo
 * @brief string.h
 * @version alpha-1.0.0
 * @date 2022-12-31
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

#include <stddef.h>

// memset
inline static void memset(void *dst, int val, size_t size) {
	// 还是用C写不容易出问题
	for (int i = 0 ; i < size ; i ++) {
		*(char *)(dst + i) = val & 0xFF;
	}
}

// memcpy
inline static void memcpy(void *dst, const void *src, size_t size) {
	// 还是用C写不容易出问题
	for (int i = 0 ; i < size ; i ++) {
		*(char *)(dst + i) = *(char *)(src + i);
	}
}

// 比较字符串
inline static int strcmp(const char *str1, const char *str2) {
	int ret = 0;
	// 挨个匹配
	do {
		ret = *(str1) - *(str2);
		// 移动指针
		str1 ++;
		str2 ++;
	} while (*str1 != 0 && *str2 != 0 && ret == 0);

	// *str2 = 0而*str1 != 0 => len(str1) > len(str2) => str1 > str2
	if (*str1 != 0) return 1;
	else if (*str2 != 0) return -1;

	// 根据ret符号获得比较结果
	if (ret < 0) return -1;
	else if (ret > 0) return 1;
	// 相等
	return 0;
}

//计算str长度
inline static int strlen(const char *str) {
	// 将cnt设置为INT_MAX 通过cnt的减少量获得str的长度
	int cnt = 0x7FFFFFFF;
    //使用字符串扫描指令
    __asm__ ("\
	cld\n\
    repnz\n\
    scasb" : "+c" (cnt) : "D" (str), "a" (0));
    return 0x7FFFFFFF - cnt - 1;
}

// 复制字符串
inline static char *strcpy(void *dst, const char *src) {
	char *_dst = (char *)dst;
	// 挨个复制
	while (*src != 0) {
		*_dst = *src;
		// 移动指针
		_dst ++;
		src ++;
	}
	return dst;
}

// 查找字符
inline static char *strchr(const char *str, char ch) {
	// 挨个查找
	while (*str != 0) {
		if (*str == ch) {
			return (char *)str;
		}
	}
	// 未查找到对应字符
	return NULL;
}

// 匹配字符串(前count个字符)
inline static int strncmp(const char *str1, const char *str2, int count){
	int ret = 0;
	// 挨个匹配
	do {
		ret = *(str1) - *(str2);
		// 移动指针
		str1 ++;
		str2 ++;
		count --;
	} while (*str1 != 0 && *str2 != 0 && ret == 0 && count > 0);
	// 根据ret符号获得比较结果
	if (ret < 0) return -1;
	else if (ret > 0) return 1;
	// 相等
	return 0;
}

// 复制字符串(前count个字符)
inline static char *strncpy(char *dst, const char *src, int count) {
	char *_dst = (char *)dst;
	// 挨个复制
	while (*src != 0 && count > 0) {
		*_dst = *src;
		// 移动指针
		_dst ++;
		src ++;
		count --;
	}
	return dst;
}

// 查找字符(末次出现)
inline static char *strrchr(const char *str, char ch){
	char *ret = NULL;
	// 挨个查找
	while (*str != 0) {
		if (*str == ch) {
			// 覆盖上次的结果
			ret = (char *)str;
		}
	}
	return ret;
}

// 返回str从开头开始连续在accept中出现过的字符的个数
// 时间复杂度O(S+A)
inline static int strspn(const char *str, const char *accept) {
	// 记录accept中的字符
	char map[128] = {};
	while ((int)*accept != 0) {
		//将accept中出现过的字符在map中标记
		map[(int)*accept] = 1;
		accept ++;
	}
	int cnt = 0;
	while (*str != 0) {
		// 查表
		if (! map[(int)*str]) {
			break;
		}
		cnt ++;
	}
	return cnt;
}

// 返回str从开头开始连续没在accept中出现过的字符的个数
// 时间复杂度O(S+A)
inline static int strcspn(const char *str, const char *accept) {
	// 记录accept中的字符
	char map[128] = {};
	while ((int)*accept != 0) {
		//将accept中出现过的字符在map中标记
		map[(int)*accept] = 1;
		accept ++;
	}
	int cnt = 0;
	while (*str != 0) {
		// 查表
		if (map[(int)*str]) {
			break;
		}
		cnt ++;
	}
	return cnt;
}

// 返回str在accept中出现过的第一个字符的指针
inline static char *strpbrk(const char *str, const char *accept) {
	return ((char *)str) + strcspn(str, accept);
}

// 将src拼接在dst上
inline static char *strcat(char *dst, const char *src) {
	// 获取dst结尾指针
	char *cat_pos = dst + strlen(dst);
	// 挨个复制
	while (*src != 0) {
		*cat_pos = *src;
		// 移动指针
		cat_pos ++;
		src ++;
	}
	return dst;
}

// 将src的前count个字符拼接在dst上
inline static char *strncat(char *dst, const char *src, int count) {
	// 获取dst结尾指针
	char *cat_pos = dst + strlen(dst);
	// 挨个复制
	while (*src != 0 && count > 0) {
		*cat_pos = *src;
		// 移动指针
		cat_pos ++;
		src ++;
		count --;
	}
	return dst;
}

// 分割字符串
// TMD这玩意谁tmd爱写谁tmd写去
inline static char *strtok(char *str, const char *split) {
	//摆烂啦哈哈哈哈不写啦
	return NULL;
}

// 从src中复制size个字节到dst中
inline static void *memmove(void *dst, const void *src, size_t size) {
	// 判断是正向复制还是反向复制
	if (dst < src) {
		// 正向复制
		__asm__("\
	cld\n\
	rep\n\
	movsb" :: "c"(size), "S"(src), "D"(dst) : "memory"
		);
	}
	else {
		// 反向复制
		__asm__("\
	std\n\
	rep\n\
	movsb" :: "c"(size), "S"(src + size - 1), "D"(dst + size - 1) : "memory"
		);
	}
	return dst;
}

// 比较内存中前count个字节
inline static int memcmp(const void *str1, const void *str2, int count) {
	int ret = 0;
	const char *_str1 = str1, *_str2 = str2;
	// 挨个比较
	do {
		ret = *(_str1) - *(_str2);
		// 移动指针
		str1 ++;
		str2 ++;
		count --;
	} while (ret == 0 && count > 0);
	// 根据ret符号获得比较结果
	if (ret < 0) return -1;
	else if (ret > 0) return 1;
	// 相等
	return 0;
}

// 在内存的前count个字节中查找ch
inline static void *memchr(const void *str, char ch, int count) {
	char *_str = (char *)str;
	// 挨个查找
	while (count > 0) {
		if (*_str == ch) {
			return _str;
		}
		count --;
	}
	// 未查找到
	return NULL;
}