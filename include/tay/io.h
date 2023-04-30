/* 
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * io.h
 *
 * In&Out
 *
 */



#pragma once

#include <tay/types.h>

static inline byte inb(word port) { //in字节
    byte data;
    asm volatile ("inb %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

static inline void outb(word port, byte data) { //out字节
    asm volatile ("outb %0, %1" : : "a"(data), "dN"(port));
}

static inline word inw(word port) { //in字
    word data;
    asm volatile ("inw %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

static inline void outw(word port, word data) { //out字
    asm volatile ("outw %0, %1" : : "a"(data), "dN"(port));
}

static inline dword ind(word port) { //in双字
    dword data;
    asm volatile ("inl %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

static inline void outd(word port, dword data) { //out双字
    asm volatile ("outl %0, %1" : : "a"(data), "dN"(port));
}

static inline sreg_t rdcs(void) { //读取cs值
    sreg_t reg;
    asm volatile ("movw %%cs, %0" : "=rm"(reg));
    return reg;
}

static inline b16 rdds(void) { //读取ds值
    sreg_t reg;
    asm volatile ("movw %%ds, %0" : "=rm"(reg));
    return reg;
}

static inline void stds(sreg_t reg) {
    asm volatile ("movw %0, %%ds" : : "rm"(reg));
}

static inline sreg_t rdes(void) { //读取es值
    sreg_t reg;
    asm volatile ("movw %%es, %0" : "=rm"(reg));
    return reg;
}

static inline void stes(sreg_t reg) {
    asm volatile ("movw %0, %%es" : : "rm"(reg));
}

static inline sreg_t rdfs(void) { //读取fs值
    sreg_t reg;
    asm volatile ("movw %%fs, %0" : "=rm"(reg));
    return reg;
}

static inline void stfs(sreg_t reg) {
    asm volatile ("movw %0, %%fs" : : "rm"(reg));
}

static inline sreg_t rdgs(void) { //读取gs值
    sreg_t reg;
    asm volatile ("movw %%gs, %0" : "=rm"(reg));
    return reg;
}

static inline void stgs(sreg_t reg) {
    asm volatile ("movw %0, %%gs" : : "rm"(reg));
}

static inline sreg_t rdss(void) { //读取ss值
    sreg_t reg;
    asm volatile ("movw %%ss, %0" : "=rm"(reg));
    return reg;
}

static inline void stss(sreg_t reg) {
    asm volatile ("movw %0, %%ss" : : "rm"(reg));
}