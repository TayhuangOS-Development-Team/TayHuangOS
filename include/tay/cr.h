/**
 * @file cr.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 控制寄存器
 * @version alpha-1.0.0
 * @date 2023-08-20
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

#include <tay/types.h>

/**
 * @brief CR0寄存器
 *
 */
typedef struct {
    /**
     * @brief Protection Enable(保护模式启用) bit 0
     * 当PE=1时启用保护模式
     * 当PE=0时启用实模式
     */
    bool PE : 1;
    /**
     * @brief Monitor Coproccesor(协处理器监视) bit 1
     * 与 EM/TS相关
     *
     */
    bool MP : 1;
    /**
     * @brief Emulation(模拟) bit 2
     * 用于模拟数学协处理器
     * 当EM=1时 使用浮点运算指令将会导致[#NM]异常
     * 令EM=1可以让软件模拟FPU执行浮点运算指令
     */
    bool EM : 1;
    /**
     * @brief Task Switched(进程切换) bit 3
     * 允许FPU直到新进程访问其时再保存现场
     * 当TS=1时 使用浮点运算指令将会导致[#NM]异常
     * 当TS=1且MP=1时 wait/fwait函数也会导致[#NM]异常
     *
     */
    bool TS : 1;
    /**
     * @brief Extension Type(拓展类型) bit 4
     * 在80386与80486中 用于标识Intel 387 DX协处理器的存在
     * Pentium CPU之后保留
     * ET外星人(x)
     */
    bool ET : 1;
    /**
     * @brief Numeric Error(数学错误) bit 5
     * NE=1时 使用本机FPU错误报告机制
     * NE=0时 使用PC式FPU错误报告机制
     *
     */
    bool NE : 1;
    /** 保留 bit 6~15 */
    word reserved3 : 10;
    /**
     * @brief Write Protection(写保护) bit 16
     * WP=1时 阻止高特权级向低特权级写入
     *
     */
    bool WP : 1;
    /** 保留 bit 17 */
    byte reserved2 : 1;
    /**
     * @brief Alignment Mask(对齐掩码) bit 18
     * AM=1时 启用Alignment Check[#AC](对齐检查)
     * 当EFLAGS/RFLAGS中AC=1且CPL=3时 进行对齐检查
     *
     */
    bool AM : 1;
    /** 保留 bit 19~28 */
    word reserved : 10;
    /**
     * @brief Not Write Through(不直写) bit 29
     * NW=1时 缓存不使用直写方式
     */
    bool NW : 1;
    /**
     * @brief Cache Disable(缓存禁用) bit 30
     * CD=1时 不启用缓存
     *
     */
    bool CD : 1;
    /**
     * @brief Paging(分页) bit 31
     * PG=1时 启用分页
     *
     */
    bool PG : 1;
} CR0;

/**
 * @brief CR2寄存器
 *
 */
typedef struct {
    /**
     * @brief Page Fault Linear Address(页错误线性地址)
     *
     */
#if BITS==32
    dword PFLA;
#else
    qword PFLA;
#endif
} CR2;

/**
 * @brief CR3寄存器
 *
 */
typedef union cr3_t {
    struct {
        /** 保留 bit 0~2 */
        byte reserved2 : 3;
        /**
         * @brief Page-level Writes Transparent(页级别写透明) bit 3
         * PWT=1时 缓存使用直写方式写页
         * PWT=0时 缓存使用写回方式写页
         *
         */
        bool PWT : 1;
        /**
         * @brief Page-level Cache Disable(页级别缓存禁用) bit 4
         * PCD=1时 禁止使用缓存
         *
         */
        bool PCD : 1;
        /** 保留 bit 5~11*/
        byte reserved3 : 7;
        /** 页表地址 bit 12~50 */
        qword _pageEntry : 39;
        /** 保留 bit 51~63 */
        word reserved : 13;
    };
    /** 页表地址 */
    qword pageEntry;
} CR3;

/** CR3页表地址掩码 */
#define CR3_PAGE_ENTRY_MASK (0xFFFFF000)

/**
 * @brief CR4寄存器
 *
 */
typedef struct {
    /**
     * @brief Virtual-8086 Mode Extensions(虚拟8086模式拓展) bit 0
     * VME=1时 启用虚拟8086模式下的异常与终端处理
     *
     */
    bool VME : 1;
    /**
     * @brief Protected-Mode Virtual Interrupts(保护模式虚拟中断) bit 1
     * PVI=1时 启用VIF位
     *
     */
    bool PVI : 1;
    /**
     * @brief Time Stamp Disable(禁用时间戳) bit 2
     * TSD=1时 只允许0特权级使用RDTSC指令
     *
     */
    bool TSD : 1;
    /**
     * @brief Debugging Extensions(调试拓展) bit 3
     * DE=1时 对调试寄存器DR4与DR5的引用会导致[#UD]异常
     * DE=0时 CPU会启用别名来引用DR4与DR5
     *
     */
    bool DE : 1;
    /**
     * @brief Page Size Extensions(页大小拓展) bit 4
     * PSE=1时 启用4MB页面
     *
     */
    bool PSE : 1;
    /**
     * @brief Physical Address Extension(物理地址拓展) bit 5
     * PAE=1时 允许分页机制引用36位物理地址
     *
     */
    bool PAE : 1;
    /**
     * @brief Machine-Check Enable(机器检查启用) bit 6
     * MCE=1时 启用Machine Check[#MC](机器检查)
     *
     */
    bool MCE : 1;
    /**
     * @brief Page Global Enable(全局页启用) bit 7
     * PGE=1时 启用全局页机制
     *
     */
    bool PGE : 1;
    /**
     * @brief Performance-Monitoring Counter Enable(性能监视计数器启用) bit 8
     * PCE=1时 任何特权级均能使用RDPMC指令
     * PCE=0时 只有0特权级能使用RDPMC指令
     *
     */
    bool PCE : 1;
    /**
     * @brief Operating Sytsem FXSAVE/FXRSTOR Support(操作系统FXSAVE/FXRSTOR支持) bit 9
     * OSFXSR=1表示系统与CPU均支持在任务切换时使用FXSAVE/FXRSTOR指令
     *
     */
    bool OSFXSR : 1;
    /**
     * @brief Operating System Unmasked Exception Support(操作系统无掩码异常支持) bit 10
     * OSFXSR=1表示系统支持处理SIMD无掩码异常
     *
     */
    bool OSXMMEXCPT : 1;
    /**
     * @brief User-Mode Instruction Prevention(用户模式指令禁止) bit 11
     * 当UMIP=1时 SGDT, SIDT, SLDT, SMSW, STR 指令在CPL>0时使用会导致异常
     *
     */
    bool UMIP : 1;
    /**
     * @brief 57-bit Linear Addresses(57位线性地址) bit 12
     * LA57=1时 在IA32-e模式下使用5级页表访问57位线性地址
     * LA57=0时 在IA32-e模式下使用4级页表访问48位线性地址
     *
     */
    bool LA57 : 1;
    /**
     * @brief VMX-Enable Bit(VMX启用位) bit 13
     * VMXE=1时 启用VMX操作
     *
     */
    bool VMXE : 1;
    /**
     * @brief SMX-Enable Bit(SMX启用位) bit 14
     * SMXE=1时 启用SMX操作
     *
     */
    bool SMXE : 1;
    /** 保留 bit 15 */
    byte reserved2 : 1;
    /**
     * @brief FSGSBASE-Enable Bit(FSGS基启用位) bit 16
     * FSGSBASE=1时 启用RDFSBASE, RDGSBASE, WRFSBASE, WRGSBASE指令
     *
     */
    bool FSGSBASE : 1;
    /**
     * @brief PCID-Enable Bit(PCID启用位) bit 17
     * PCIDE=1时 启用进程上下文识别符
     *
     */
    bool PCIDE : 1;
    /**
     * @brief Operating System XSAVE and Processor Extended States-Enable Bit(操作系统XSAVE与处理器拓展状态启用位) bit 18
     * OSXSAVE=1时
     * (1) 指示(通过 CPUID.01H:ECX.OSXSAVE[bit 27])操作系统支持普通软件使用XGETBV, XSAVE, XRSTOR指令;
     * (2) 启用XSAVE与XRSTOR指令 来保存x87 FPU状态(包括MMX寄存器), SSE状态(XMM寄存器与MXCSR寄存器),与XCR0寄存器中的其他拓展状态;
     * (3) 允许处理器按顺序执行XGETBV和XSETBV执行来读写XCR0寄存器
     *
     */
    bool OSXSAVE : 1;
    /**
     * @brief Key-Locker Enable Bit(匙锁启用位) bit 19
     * KL=1时 启用Key-Locker指令 若系统激活AES Key-Locker指令 则CPUID.19H:EBX.AESKLE[bit 0]=1且启用AES Key-Locker指令
     * KL=0时 CPUID.19H:EBX.AESKLE[bit 0]=1且使用AES Key-Locker指令会导致[#UD]异常
     *
     */
    bool KL : 1;
    /**
     * @brief SMEP Enable Bit(SMEP启用位) bit 20
     * SMEP=1时 启用管理模式执行保护机制
     *
     */
    bool SMEP : 1;
    /**
     * @brief SMAP Enable Bit(SMAP启用位) bit 21
     * SMAP=1时 启用管理模式访问保护机制
     *
     */
    bool SMAP : 1;
    /**
     * @brief Enable Protection Keys for User-mode Pages(为用户模式页面启用保护密钥) bit 22
     * PKE=1时 启用用户模式页面保护密钥
     *
     */
    bool PKE : 1;
    /**
     * @brief Control-flow Enforcement Technology(控制流强迫技术) bit 23
     * CET=1时 启用CET技术
     *
     */
    bool CET : 1;
    /**
     * @brief Enable Protection Keys for Supervisor-mode Pages(为管理模式页面启用保护密钥) bit 24
     * PKE=1时 启用管理模式页面保护密钥
     *
     */
    bool PKS : 1;
    /**
     * @brief User Interrupts Enable Bit(用户中断启用位) bit 25
     * UINTR=1时 启用用户中断
     *
     */
    bool UINTR : 1;
    /** 保留 bit 26~31*/
    word reserved3 : 6;
} CR4;

/**
 * @brief EFER寄存器
 *
 */
typedef struct {
    /**
     * @brief Syscall Enable(Syscall指令启用) bit0
     * SCE=1时 启用Syscall指令
     *
     */
    bool SCE : 1;
    /** 保留 bit 1~7 */
    byte reserved1 : 7;
    /**
     * @brief Long Mode Enable(长模式启用) bit 8
     * LME=1时 启用长模式
     *
     */
    bool LME : 1;
    /** 保留 bit 9 */
    byte reserved2 : 1;
    /**
     * @brief Long Mode Active(长模式激活) bit 10
     * LMA=1时 说明处于长模式
     *
     */
    bool LMA : 1;
    /**
     * @brief Execute Disable Bit Enable(禁止执行位启用) bit 11
     * NXE=1时 启用禁止执行位
     *
     */
    bool NXE : 1;
    /** 保留 12~63 */
    qword reserved3 : 52;
} EFER;

/** EFER地址 */
#define MSR_EFER_ADDR (0xC0000080)

/**
 * @brief 读取CR0
 *
 * @return CR0
 */
static inline CR0 rdcr0(void) {
    CR0 cr0;
    dword cr0Val;

    asm volatile("movl %%cr0, %0" : "=r"(cr0Val));

    cr0 = *(CR0 *)&cr0Val;

    return cr0;
}

/**
 * @brief 写CR0
 *
 * @param cr0 CR0
 */
static inline void wrcr0(CR0 cr0) {
    dword cr0Val = *(dword *)&cr0;

    asm volatile("movl %0, %%cr0" : : "r"(cr0Val));
}

/**
 * @brief 读取CR2
 *
 * @return CR2
 */
static inline CR2 rdcr2(void) {
    CR2 cr2;
    dword cr2Val;

    asm volatile("movl %%cr2, %0" : "=r"(cr2Val));

    cr2 = *(CR2 *)&cr2Val;

    return cr2;
}

/**
 * @brief 写CR2
 *
 * @param cr2 CR2
 */
static inline void wrcr2(CR2 cr2) {
    dword cr2Val = *(dword *)&cr2;

    asm volatile("movl %0, %%cr2" : : "r"(cr2Val));
}

/**
 * @brief 读取CR3
 *
 * @return CR3
 */
static inline CR3 rdcr3(void) {
    CR3 cr3;
    dword cr3Val;

    asm volatile("movl %%cr3, %0" : "=r"(cr3Val));

    cr3 = *(CR3 *)&cr3Val;

    return cr3;
}

/**
 * @brief 写CR3
 *
 * @param cr3 CR3
 */
static inline void wrcr3(CR3 cr3) {
    dword cr3Val = *(dword *)&cr3;

    asm volatile("movl %0, %%cr3" : : "r"(cr3Val));
}

/**
 * @brief 读取CR4
 *
 * @return CR4
 */
static inline CR4 rdcr4(void) {
    CR4 cr4;
    dword cr4Val;

    asm volatile("movl %%cr4, %0" : "=r"(cr4Val));

    cr4 = *(CR4 *)&cr4Val;

    return cr4;
}

/**
 * @brief 写CR4
 *
 * @param cr4 CR4
 */
static inline void wrcr4(CR4 cr4) {
    dword cr4Val = *(dword *)&cr4;

    asm volatile("movl %0, %%cr4" : : "r"(cr4Val));
}

/**
 * @brief 读MSR
 *
 * @param address MSR地址
 * @return MSR
 */
static inline qword rdmsr(dword address) {
    // EAX:EDX -> high:low
    dword high;
    dword low;
    // 读取msr
    asm volatile("rdmsr" : "=d"(high), "=a"(low) : "c"(address));
    qword msr = (((qword)high) << 32) | ((qword)low);
    return msr;
}

/**
 * @brief 写MSR
 *
 * @param address MSR地址
 * @param msr MSR
 */
static inline void wrmsr(dword address, qword msr) {
    // high:low -> EAX:EDX
    dword high = (msr >> 32) & 0xFFFFFFFF;
    dword low  = (msr      ) & 0xFFFFFFFF;
    // 写msr
    asm volatile("wrmsr" : : "d"(high), "a"(low), "c"(address));
}

/**
 * @brief 读EFER
 *
 * @return EFER
 */
static inline EFER rdefer(void) {
    EFER efer;
    qword eferVal = rdmsr(MSR_EFER_ADDR);
    efer = *(EFER *)(&eferVal);
    return efer;
}

/**
 * @brief 写EFER
 *
 * @param efer EFER
 */
static inline void wrefer(EFER efer) {
    qword eferVal = *(qword *)&efer;
    wrmsr(MSR_EFER_ADDR, eferVal);
}