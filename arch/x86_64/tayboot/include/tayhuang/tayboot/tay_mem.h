/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * tay_mem.h
 *
 * 基础的内存函数
 *
 */



#include <tayboot/tay_type.h>

static inline sreg_t rdcs(void) { //读取cs值
    sreg_t reg;
    asmv ("movw %%cs, %0" : "=rm"(reg));
    return reg;
}

static inline b16 rdds(void) { //读取ds值
    sreg_t reg;
    asmv("movw %%ds, %0" : "=rm"(reg));
    return reg;
}

static inline sreg_t rdes(void) { //读取es值
    sreg_t reg;
    asmv("movw %%es, %0" : "=rm"(reg));
    return reg;
}

static inline sreg_t rdfs(void) { //读取fs值
    sreg_t reg;
    asmv("movw %%fs, %0" : "=rm"(reg));
    return reg;
}

static inline sreg_t rdgs(void) { //读取gs值
    sreg_t reg;
    asmv("movw %%gs, %0" : "=rm"(reg));
    return reg;
}

static inline sreg_t rdss(void) { //读取ss值
    sreg_t reg;
    asmv("movw %%ss, %0" : "=rm"(reg));
    return reg;
}

static inline void stds(sreg_t reg) { //设置ds值
    asmv("movw %0, %%ds" : : "rm"(reg));
}

static inline void stes(sreg_t reg) { //设置es值
    asmv("movw %0, %%es" : : "rm"(reg));
}

static inline void stfs(sreg_t reg) { //设置fs值
    asmv("movw %0, %%fs" : : "rm"(reg));
}

static inline void stgs(sreg_t reg) { //设置gs值
    asmv("movw %0, %%gs" : : "rm"(reg));
}

static inline b8 rdds8(addr_t addr) { //读byte ds:[addr]
    b8 val;
    asmv("movb %%ds:%1, %0" : "=q"(val) : "m"(*(b8*)addr));
    return val;
}

static inline b8 rdes8(addr_t addr) { //读byte es:[addr]
    b8 val;
    asmv("movb %%es:%1, %0" : "=q"(val) : "m"(*(b8*)addr));
    return val;
}

static inline b8 rdfs8(addr_t addr) { //读byte fs:[addr]
    b8 val;
    asmv("movb %%fs:%1, %0" : "=q"(val) : "m"(*(b8*)addr));
    return val;
}

static inline b8 rdgs8(addr_t addr) { //读byte gs:[addr]
    b8 val;
    asmv("movb %%gs:%1, %0" : "=q"(val) : "m"(*(b8*)addr));
    return val;
}

static inline b16 rdds16(addr_t addr) { //读word ds:[addr]
    b16 val;
    asmv("movw %%ds:%1, %0" : "=r"(val) : "m"(*(b16*)addr));
    return val;
}

static inline b16 rdes16(addr_t addr) { //读word es:[addr]
    b16 val;
    asmv("movw %%es:%1, %0" : "=r"(val) : "m"(*(b16*)addr));
    return val;
}

static inline b16 rdfs16(addr_t addr) { //读word fs:[addr]
    b16 val;
    asmv("movw %%fs:%1, %0" : "=r"(val) : "m"(*(b16*)addr));
    return val;
}

static inline b16 rdgs16(addr_t addr) { //读word gs:[addr]
    b16 val;
    asmv("movw %%gs:%1, %0" : "=r"(val) : "m"(*(b16*)addr));
    return val;
}

static inline b32 rdds32(addr_t addr) { //读dword ds:[addr]
    b32 val;
    asmv("movl %%ds:%1, %0" : "=r"(val) : "m"(*(b32*)addr));
    return val;
}

static inline b32 rdes32(addr_t addr) { //读dword es:[addr]
    b32 val;
    asmv("movl %%es:%1, %0" : "=r"(val) : "m"(*(b32*)addr));
    return val;
}

static inline b32 rdfs32(addr_t addr) { //读dword fs:[addr]
    b32 val;
    asmv("movl %%fs:%1, %0" : "=r"(val) : "m"(*(b32*)addr));
    return val;
}

static inline b32 rdgs32(addr_t addr) { //读dword gs:[addr]
    b32 val;
    asmv("movl %%gs:%1, %0" : "=r"(val) : "m"(*(b32*)addr));
    return val;
}

static inline void stds8(addr_t addr, b8 val) { //写byte ds:[addr]
    asmv("movb %1, %%ds:%0": "+m"(*(b8*)addr) : "qi"(val));
}

static inline void stes8(addr_t addr, b8 val) { //写byte es:[addr]
    asmv("movb %1, %%es:%0": "+m"(*(b8*)addr) : "qi"(val));
}

static inline void stfs8(addr_t addr, b8 val) { //写byte fs:[addr]
    asmv("movb %1, %%fs:%0": "+m"(*(b8*)addr) : "qi"(val));
}

static inline void stgs8(addr_t addr, b8 val) { //写byte gs:[addr]
    asmv("movb %1, %%gs:%0": "+m"(*(b8*)addr) : "qi"(val));
}

static inline void stds16(addr_t addr, b16 val) { //写word ds:[addr]
    asmv("movw %1, %%ds:%0": "+m"(*(b16*)addr) : "ri"(val));
}

static inline void stes16(addr_t addr, b16 val) { //写word es:[addr]
    asmv("movw %1, %%es:%0": "+m"(*(b16*)addr) : "ri"(val));
}

static inline void stfs16(addr_t addr, b16 val) { //写word fs:[addr]
    asmv("movw %1, %%fs:%0": "+m"(*(b16*)addr) : "ri"(val));
}

static inline void stgs16(addr_t addr, b16 val) { //写word gs:[addr]
    asmv("movw %1, %%gs:%0": "+m"(*(b16*)addr) : "ri"(val));
}

static inline void stds32(addr_t addr, b32 val) { //写dword ds:[addr]
    asmv("movl %1, %%ds:%0": "+m"(*(b32*)addr) : "ri"(val));
}

static inline void stes32(addr_t addr, b32 val) { //写dword es:[addr]
    asmv("movl %1, %%es:%0": "+m"(*(b32*)addr) : "ri"(val));
}

static inline void stfs32(addr_t addr, b32 val) { //写dword fs:[addr]
    asmv("movl %1, %%fs:%0": "+m"(*(b32*)addr) : "ri"(val));
}

static inline void stgs32(addr_t addr, b32 val) { //写dword gs:[addr]
    asmv("movl %1, %%gs:%0": "+m"(*(b32*)addr) : "ri"(val));
}