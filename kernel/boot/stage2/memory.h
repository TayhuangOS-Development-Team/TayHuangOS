#pragma once

//-------ds------
static inline word rdds(void) {
    word seg;
#ifndef _WIN32
    asm("movw %%ds, %0" : "=rm" (seg));
#endif
    return seg;
}
static inline void stds(word seg) {
#ifndef _WIN32
    asm("movw %0, %%ds" : : "rm" (seg));
#endif
}
static inline byte rdds8(address addr) {
	byte data;
#ifndef _WIN32
	asm volatile("movb %%ds:%1, %0" : "=q" (data) : "m" (*(byte *)addr));
#endif
	return data;
}
static inline word rdds16(address addr) {
	word data;
#ifndef _WIN32
	asm volatile("movw %%ds:%1, %0" : "=r" (data) : "m" (*(word *)addr));
#endif
	return data;
}
static inline dword rdds32(address addr) {
	dword data;
#ifndef _WIN32
	asm volatile("movl %%ds:%1, %0" : "=r" (data) : "m" (*(dword *)addr));
#endif
	return data;
}
static inline void wrds8(byte data, address addr) {
#ifndef _WIN32
	asm volatile("movb %1,%%ds: %0" : "+m" (*(byte *)addr) : "qi" (data));
#endif
}
static inline void wrds16(word data, address addr) {
#ifndef _WIN32
	asm volatile("movw %1,%%ds: %0" : "+m" (*(word *)addr) : "ri" (data));
#endif
}
static inline void wrds32(dword data, address addr) {
#ifndef _WIN32
	asm volatile("movl %1,%%ds: %0" : "+m" (*(dword *)addr) : "ri" (data));
#endif
}
static inline void rddsn(byte* data, int len, address addr) {
    for (int i = 0 ; i < len ; i ++) {
        data[i] = rdds8(addr + i);
    }
}
static inline void wrdsn(byte* data, int len, address addr) {
    for (int i = 0 ; i < len ; i ++) {
        wrds8(data[i], addr + i);
    }
}
//-------gs------
static inline word rdgs(void) {
    word seg;
#ifndef _WIN32
    asm("movw %%gs, %0" : "=rm" (seg));
#endif
    return seg;
}
static inline void stgs(word seg) {
#ifndef _WIN32
    asm("movw %0, %%gs" : : "rm" (seg));
#endif
}
static inline byte rdgs8(address addr) {
	byte data;
#ifndef _WIN32
	asm volatile("movb %%gs:%1, %0" : "=q" (data) : "m" (*(byte *)addr));
#endif
	return data;
}
static inline word rdgs16(address addr) {
	word data;
#ifndef _WIN32
	asm volatile("movw %%gs:%1, %0" : "=r" (data) : "m" (*(word *)addr));
#endif
	return data;
}
static inline dword rdgs32(address addr) {
	dword data;
#ifndef _WIN32
	asm volatile("movl %%gs:%1, %0" : "=r" (data) : "m" (*(dword *)addr));
#endif
	return data;
}
static inline void wrgs8(byte data, address addr) {
#ifndef _WIN32
	asm volatile("movb %1,%%gs: %0" : "+m" (*(byte *)addr) : "qi" (data));
#endif
}
static inline void wrgs16(word data, address addr) {
#ifndef _WIN32
	asm volatile("movw %1,%%gs: %0" : "+m" (*(word *)addr) : "ri" (data));
#endif
}
static inline void wrgs32(dword data, address addr) {
#ifndef _WIN32
	asm volatile("movl %1,%%gs: %0" : "+m" (*(dword *)addr) : "ri" (data));
#endif
}
static inline void rdgsn(byte* data, int len, address addr) {
    for (int i = 0 ; i < len ; i ++) {
        data[i] = rdgs8(addr + i);
    }
}
static inline void wrgsn(byte* data, int len, address addr) {
    for (int i = 0 ; i < len ; i ++) {
        wrgs8(data[i], addr + i);
    }
}
//-------es------
static inline word rdes(void) {
    word seg;
#ifndef _WIN32
    asm("movw %%es, %0" : "=rm" (seg));
#endif
    return seg;
}
static inline void stes(word seg) {
#ifndef _WIN32
    asm("movw %0, %%es" : : "rm" (seg));
#endif
}
static inline byte rdes8(address addr) {
	byte data;
#ifndef _WIN32
	asm volatile("movb %%es:%1, %0" : "=q" (data) : "m" (*(byte *)addr));
#endif
	return data;
}
static inline word rdes16(address addr) {
	word data;
#ifndef _WIN32
	asm volatile("movw %%es:%1, %0" : "=r" (data) : "m" (*(word *)addr));
#endif
	return data;
}
static inline dword rdes32(address addr) {
	dword data;
#ifndef _WIN32
	asm volatile("movl %%es:%1, %0" : "=r" (data) : "m" (*(dword *)addr));
#endif
	return data;
}
static inline void wres8(byte data, address addr) {
#ifndef _WIN32
	asm volatile("movb %1,%%es: %0" : "+m" (*(byte *)addr) : "qi" (data));
#endif
}
static inline void wres16(word data, address addr) {
#ifndef _WIN32
	asm volatile("movw %1,%%es: %0" : "+m" (*(word *)addr) : "ri" (data));
#endif
}
static inline void wres32(dword data, address addr) {
#ifndef _WIN32
	asm volatile("movl %1,%%es: %0" : "+m" (*(dword *)addr) : "ri" (data));
#endif
}
static inline void rdesn(byte* data, int len, address addr) {
    for (int i = 0 ; i < len ; i ++) {
        data[i] = rdes8(addr + i);
    }
}
static inline void wresn(byte* data, int len, address addr) {
    for (int i = 0 ; i < len ; i ++) {
        wres8(data[i], addr + i);
    }
}
//-------fs------
static inline word rdfs(void) {
    word seg;
#ifndef _WIN32
    asm("movw %%fs, %0" : "=rm" (seg));
#endif
    return seg;
}
static inline void stfs(word seg) {
#ifndef _WIN32
    asm("movw %0, %%fs" : : "rm" (seg));
#endif
}
static inline byte rdfs8(address addr) {
	byte data;
#ifndef _WIN32
	asm volatile("movb %%fs:%1, %0" : "=q" (data) : "m" (*(byte *)addr));
#endif
	return data;
}
static inline word rdfs16(address addr) {
	word data;
#ifndef _WIN32
	asm volatile("movw %%fs:%1, %0" : "=r" (data) : "m" (*(word *)addr));
#endif
	return data;
}
static inline dword rdfs32(address addr) {
	dword data;
#ifndef _WIN32
	asm volatile("movl %%fs:%1, %0" : "=r" (data) : "m" (*(dword *)addr));
#endif
	return data;
}
static inline void wrfs8(byte data, address addr) {
#ifndef _WIN32
	asm volatile("movb %1,%%fs: %0" : "+m" (*(byte *)addr) : "qi" (data));
#endif
}
static inline void wrfs16(word data, address addr) {
#ifndef _WIN32
	asm volatile("movw %1,%%fs: %0" : "+m" (*(word *)addr) : "ri" (data));
#endif
}
static inline void wrfs32(dword data, address addr) {
#ifndef _WIN32
	asm volatile("movl %1,%%fs: %0" : "+m" (*(dword *)addr) : "ri" (data));
#endif
}
static inline void rdfsn(byte* data, int len, address addr) {
    for (int i = 0 ; i < len ; i ++) {
        data[i] = rdfs8(addr + i);
    }
}
static inline void wrfsn(byte* data, int len, address addr) {
    for (int i = 0 ; i < len ; i ++) {
        wrfs8(data[i], addr + i);
    }
}