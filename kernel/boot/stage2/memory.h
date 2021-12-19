
#include "types.h"

static inline sreg_t rdcs(void) {
    sreg_t reg;
    asm volatile("movw %%cs, %0" : "=rm"(reg));
    return reg;
}

static inline b16 rdds(void) {
    sreg_t reg;
    asm volatile("movw %%ds, %0" : "=rm"(reg));
    return reg;
}

static inline sreg_t rdes(void) {
    sreg_t reg;
    asm volatile("movw %%es, %0" : "=rm"(reg));
    return reg;
}

static inline sreg_t rdfs(void) {
    sreg_t reg;
    asm volatile("movw %%fs, %0" : "=rm"(reg));
    return reg;
}

static inline sreg_t rdgs(void) {
    sreg_t reg;
    asm volatile("movw %%gs, %0" : "=rm"(reg));
    return reg;
}

static inline sreg_t rdss(void) {
    sreg_t reg;
    asm volatile("movw %%ss, %0" : "=rm"(reg));
    return reg;
}

static inline void stds(sreg_t reg) {
    asm volatile("movw %0, %%ds" : : "rm"(reg));
}

static inline void stes(sreg_t reg) {
    asm volatile("movw %0, %%es" : : "rm"(reg));
}

static inline void stfs(sreg_t reg) {
    asm volatile("movw %0, %%fs" : : "rm"(reg));
}

static inline void stgs(sreg_t reg) {
    asm volatile("movw %0, %%gs" : : "rm"(reg));
}

static inline b8 rdds8(addr_t addr) {
    b8 val;
    asm volatile("movb %%ds:%1, %0" : "=q"(val) : "m"(*(b8*)addr));
    return val;
}

static inline b8 rdes8(addr_t addr) {
    b8 val;
    asm volatile("movb %%es:%1, %0" : "=q"(val) : "m"(*(b8*)addr));
    return val;
}

static inline b8 rdfs8(addr_t addr) {
    b8 val;
    asm volatile("movb %%fs:%1, %0" : "=q"(val) : "m"(*(b8*)addr));
    return val;
}

static inline b8 rdgs8(addr_t addr) {
    b8 val;
    asm volatile("movb %%gs:%1, %0" : "=q"(val) : "m"(*(b8*)addr));
    return val;
}

static inline b16 rdds16(addr_t addr) {
    b16 val;
    asm volatile("movw %%ds:%1, %0" : "=r"(val) : "m"(*(b16*)addr));
    return val;
}

static inline b16 rdes16(addr_t addr) {
    b16 val;
    asm volatile("movw %%es:%1, %0" : "=r"(val) : "m"(*(b16*)addr));
    return val;
}

static inline b16 rdfs16(addr_t addr) {
    b16 val;
    asm volatile("movw %%fs:%1, %0" : "=r"(val) : "m"(*(b16*)addr));
    return val;
}

static inline b16 rdgs16(addr_t addr) {
    b16 val;
    asm volatile("movw %%gs:%1, %0" : "=r"(val) : "m"(*(b16*)addr));
    return val;
}

static inline b32 rdds32(addr_t addr) {
    b32 val;
    asm volatile("movl %%ds:%1, %0" : "=r"(val) : "m"(*(b32*)addr));
    return val;
}

static inline b32 rdes32(addr_t addr) {
    b32 val;
    asm volatile("movl %%es:%1, %0" : "=r"(val) : "m"(*(b32*)addr));
    return val;
}

static inline b32 rdfs32(addr_t addr) {
    b32 val;
    asm volatile("movl %%fs:%1, %0" : "=r"(val) : "m"(*(b32*)addr));
    return val;
}

static inline b32 rdgs32(addr_t addr) {
    b32 val;
    asm volatile("movl %%gs:%1, %0" : "=r"(val) : "m"(*(b32*)addr));
    return val;
}

static inline void stds8(addr_t addr, b8 val) {
    asm volatile("movb %1, %%ds:%0": "+m"(*(b8*)addr) : "qi"(val));
}

static inline void stes8(addr_t addr, b8 val) {
    asm volatile("movb %1, %%es:%0": "+m"(*(b8*)addr) : "qi"(val));
}

static inline void stfs8(addr_t addr, b8 val) {
    asm volatile("movb %1, %%fs:%0": "+m"(*(b8*)addr) : "qi"(val));
}

static inline void stgs8(addr_t addr, b8 val) {
    asm volatile("movb %1, %%gs:%0": "+m"(*(b8*)addr) : "qi"(val));
}

static inline void stds16(addr_t addr, b16 val) {
    asm volatile("movw %1, %%ds:%0": "+m"(*(b16*)addr) : "ri"(val));
}

static inline void stes16(addr_t addr, b16 val) {
    asm volatile("movw %1, %%es:%0": "+m"(*(b16*)addr) : "ri"(val));
}

static inline void stfs16(addr_t addr, b16 val) {
    asm volatile("movw %1, %%fs:%0": "+m"(*(b16*)addr) : "ri"(val));
}

static inline void stgs16(addr_t addr, b16 val) {
    asm volatile("movw %1, %%gs:%0": "+m"(*(b16*)addr) : "ri"(val));
}

static inline void stds32(addr_t addr, b32 val) {
    asm volatile("movl %1, %%ds:%0": "+m"(*(b32*)addr) : "ri"(val));
}

static inline void stes32(addr_t addr, b32 val) {
    asm volatile("movl %1, %%es:%0": "+m"(*(b32*)addr) : "ri"(val));
}

static inline void stfs32(addr_t addr, b32 val) {
    asm volatile("movl %1, %%fs:%0": "+m"(*(b32*)addr) : "ri"(val));
}

static inline void stgs32(addr_t addr, b32 val) {
    asm volatile("movl %1, %%gs:%0": "+m"(*(b32*)addr) : "ri"(val));
}