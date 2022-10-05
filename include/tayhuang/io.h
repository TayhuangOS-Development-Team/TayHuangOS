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

#include <tayhuang/types.h>

INLINE byte inb(word port) { //in字节
    byte data;
    asmv ("inb %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

INLINE void outb(word port, byte data) { //out字节
    asmv ("outb %0, %1" : : "a"(data), "dN"(port));
}

INLINE word inw(word port) { //in字
    word data;
    asmv ("inw %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

INLINE void outw(word port, word data) { //out字
    asmv ("outw %0, %1" : : "a"(data), "dN"(port));
}

INLINE dword ind(word port) { //in双字
    dword data;
    asmv ("inl %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

INLINE void outd(word port, dword data) { //out双字
    asmv ("outl %0, %1" : : "a"(data), "dN"(port));
}

INLINE sreg_t rdcs(void) { //读取cs值
    sreg_t reg;
    asmv ("movw %%cs, %0" : "=rm"(reg));
    return reg;
}

INLINE b16 rdds(void) { //读取ds值
    sreg_t reg;
    asmv("movw %%ds, %0" : "=rm"(reg));
    return reg;
}

INLINE void stds(sreg_t reg) {
    asmv("movw %0, %%ds" : : "rm"(reg));
}

INLINE sreg_t rdes(void) { //读取es值
    sreg_t reg;
    asmv("movw %%es, %0" : "=rm"(reg));
    return reg;
}

INLINE void stes(sreg_t reg) {
    asmv("movw %0, %%es" : : "rm"(reg));
}

INLINE sreg_t rdfs(void) { //读取fs值
    sreg_t reg;
    asmv("movw %%fs, %0" : "=rm"(reg));
    return reg;
}

INLINE void stfs(sreg_t reg) {
    asmv("movw %0, %%fs" : : "rm"(reg));
}

INLINE sreg_t rdgs(void) { //读取gs值
    sreg_t reg;
    asmv("movw %%gs, %0" : "=rm"(reg));
    return reg;
}

INLINE void stgs(sreg_t reg) {
    asmv("movw %0, %%gs" : : "rm"(reg));
}

INLINE sreg_t rdss(void) { //读取ss值
    sreg_t reg;
    asmv("movw %%ss, %0" : "=rm"(reg));
    return reg;
}

INLINE void stss(sreg_t reg) {
    asmv("movw %0, %%ss" : : "rm"(reg));
}
