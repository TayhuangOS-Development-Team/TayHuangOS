/**
 * @file control_registers.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 控制寄存器
 * @version alpha-1.0.0
 * @date 2022-10-22
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */



#pragma once

#include <tayhuang/attributes.h>
#include <tayhuang/types.h>

/**
 * @brief cr0寄存器结构
 * x86-64架构
 * 
 */
ARCHITECTURE_BASED typedef struct {
    /**
     * @brief 保护允许位
     * =1 => 保护模式
     * =0 => 实模式
     * 
     */
    bool pe : 1;
    /**
     * @brief 监控协处理位
     * 与第三位相关
     * MP & TS = 1 => WAIT指令产生协处理器异常
     * 
     */
    bool mp : 1;
    /**
     * @brief 模拟协处理器位
     * =1 => 禁用协处理器
     * =0 => 启用协处理器
     * 
     */
    bool em : 1;
    /**
     * @brief 任务切换位
     * =1 => 任务切换完成
     * 
     */
    bool ts : 1;
    /**
     * @brief 扩展类型位
     * =0 => 287 CO
     * =1 => 387 CO
     * 
     */
    bool et : 1;
    /**
     * @brief 协处理器错误位
     * =0 => PC形式x87报告机制
     * =1 => x87内部报告机制
     * 
     */
    bool ne : 1;
    /**
     * @brief 未使用
     * 
     */
    word reserved3 : 10;
    /**
     * @brief 写保护
     * =0 => ring0可以写入用户只读页
     * =1 => ring0不能写入用户只读页
     * 
     */
    bool wp : 1;
    /**
     * @brief 未使用
     * 
     */
    byte reserved2 : 1;
    /**
     * @brief 内存对齐检查位
     * =0 => 禁用内存对齐自动检查
     * =1 => 启用内存对齐自动检查
     * 
     */
    bool am : 1;
    /**
     * @brief 未使用
     * 
     */
    word reserved : 10;
    /**
     * @brief 直写位
     * =0 => 缓存直写
     * =1 => 缓存回写
     * 
     */
    bool nw : 1;
    /**
     * @brief 缓存禁用位
     * =0 => 启用高速缓存
     * =1 => 禁用高速缓存
     * 
     */
    bool cd : 1;
    /**
     * @brief 分页开启位
     * =0 => 关闭分页
     * =1 => 开启分页
     * 
     */
    bool pg : 1;
} cr0_t;

/**
 * @brief cr2寄存器结构
 * x86-64架构
 * 
 */
ARCHITECTURE_BASED typedef struct {
// 32位
#ifdef LOADER32BIT
    /**
     * @brief 页错误线性地址
     * 
     */
    dword pfla;
#else
    /**
     * @brief 页错误线性地址
     * 
     */
    qword pfla;
#endif
} cr2_t;

/**
 * @brief cr3寄存器结构
 * x86-64架构
 * 
 */
ARCHITECTURE_BASED typedef union cr3_t {
    struct {
        /**
         * @brief 未使用
         * 
         */
        byte reserved2 : 3;
        /**
         * @brief 直写位
         * =0 => 该页直写到缓存
         * =1 => 该页回写到缓存
         * 
         */
        bool pwt : 1;
        /**
         * @brief 缓存禁用位
         * =0 => 启用高速缓存
         * =1 => 禁用高速缓存
         * 
         */
        bool pcd : 1;
        byte reserved3 : 7;
        /**
          * @brief 页表地址
          * 
          */
        qword _page_entry : 39;
        /**
         * @brief 未使用
         * 
         */
        word reserved : 13;
    };
    /**
     * @brief 页表地址
     * 
     */
    qword page_entry;
} cr3_t;

/**
 * @brief cr4寄存器结构
 * x86-64架构
 * 
 */
ARCHITECTURE_BASED typedef struct {
    /**
     * @brief 虚拟8086位
     * =1 => 虚拟8086模式
     * 
     */
    bool vme : 1;
    /**
     * @brief 虚拟中断标志VIF启用位
     * =0 => 禁用虚拟中断标志VIF
     * =0 => 启用虚拟中断标志VIF
     * 
     */
    bool pvi : 1;
    /**
     * @brief 时间戳禁用位
     * =0 => RDTSC可以在任意权限级使用
     * =1 => RDTSC只能在ring0使用
     * 
     */
    bool tsd : 1;
    /**
     * @brief 调试拓展位
     * =1 => 别名DR4 DR5
     * 
     */
    bool de : 1;
    /**
     * @brief 大页面位
     * =1 => 4MB页&32位分页
     * 
     */
    bool pse : 1;
    /**
     * @brief 物理地址拓展位
     * =0 => =32位物理地址
     * =1 => >32位物理地址
     * 
     */
    bool pae : 1;
    /**
     * @brief 机器检查位
     * =0 => 禁用机器检查异常
     * =1 => 启用机器检查异常
     * 
     */
    bool mce : 1;
    /**
     * @brief 全局页面功能位
     * =0 => 禁用全局页面功能
     * =1 => 启用全局页面功能
     * 
     */
    bool pge : 1;
    /**
     * @brief 性能监视寄存器启用位
     * =0 => RDPMC只能在ring0使用
     * =1 => RDPMC可以在任意权限级使用
     * 
     */
    bool pce : 1;
    bool osfxsr : 1;
    bool osxmmexcpt : 1;
    /**
     * @brief 用户模式指令保护位
     * =0 => 用户模式下可使用SGDT SIDT SLDT SMSW STR
     * =1 => 用户模式下不可使用SGDT SIDT SLDT SMSW STR
     */
    bool umip : 1;;
    /**
     * @brief 未使用
     * 
     */
    byte reserved : 1;
    /**
     * @brief VMX位
     * =0 => 禁用VMX
     * =1 => 启用VMX
     * 
     */
    bool vmxe : 1;
    /**
     * @brief 未使用
     * 
     */
    byte reserved1 : 1;
    /**
     * @brief SMX位
     * =0 => 禁用SMX
     * =1 => 启用SMX
     * 
     */
    bool smxe : 1;
    bool fsgsbase : 1;
    bool pcide : 1;
    bool osxsave : 1;
    /**
     * @brief SMEP启用位
     * 
     */
    bool smep : 1;
    /**
     * @brief SMAP启用位
     * 
     */
    bool smap : 1;
    /**
     * @brief 保护密钥启用位
     * =0 => 禁用保护密钥
     * =1 => 启用保护密钥
     * 
     */
    bool pke : 1;
    /**
     * @brief 控制流执行技术位
     * =0 => 禁用控制流执行技术
     * =1 => 启用控制流执行技术
     * 
     */
    bool cet : 1;
    bool pks : 1;
    /**
     * @brief 未使用
     * 
     */
    word reserved3 : 8;
} cr4_t;

/**
 * @brief efer寄存器结构
 * x86-64架构
 * 
 */
ARCHITECTURE_BASED typedef struct {
    /**
     * @brief syscall启用位
     * =0 => 禁用syscall/sysret
     * =1 => 启用syscall/sysret
     * 
     */
    bool sce : 1;
    /**
     * @brief 未使用
     * 
     */
    byte reserved2 : 7;
    /**
     * @brief 长模式启用位
     * =0 => 禁用长模式
     * =1 => 启用长模式
     * 
     */
    bool lme : 1;
    /**
     * @brief 长模式激活位
     * =0 => 不处于长模式
     * =1 => 处于长模式
     * 
     */
    bool lma : 1;
    /**
     * @brief 页访问限制启用位
     * =0 => 禁用页访问限制
     * =1 => 启用页访问限制
     * 
     */
    bool nxe : 1;
    bool svme : 1;
    bool lmsle : 1;
    bool ffxsr : 1;
    bool tce : 1;
    qword reserved : 16;
} efer_t;

#define EFER_MSR 0xC0000080

#ifndef LOADER32BIT
#define bits qword
#else
#define bits dword
#endif

/**
 * @brief 设置cr0
 * x86-64架构
 * 
 * @param cr0 cr0寄存器值 
 */
ARCHITECTURE_BASED INLINE PUBLIC void __set_cr0(bits cr0) {
    asmv ("movq %0, %%cr0" : : "r"(cr0));
}
/**
 * @brief 获得cr0
 * x86-64架构
 * 
 * @return cr0寄存器值 
 */
ARCHITECTURE_BASED INLINE PUBLIC bits __get_cr0(void) {
    bits cr0;
    asmv ("movq %%cr0, %0" : "=r"(cr0));
    return cr0;
}
/**
 * @brief 设置cr2
 * x86-64架构
 * 
 * @param cr2 cr2寄存器值 
 */
ARCHITECTURE_BASED INLINE PUBLIC void __set_cr2(bits cr2) {
    asmv ("movq %0, %%cr2" : : "r"(cr2));
}
/**
 * @brief 获得cr2
 * x86-64架构
 * 
 * @return cr2寄存器值 
 */
ARCHITECTURE_BASED INLINE PUBLIC bits __get_cr2(void) {
    bits cr2;
    asmv ("movq %%cr2, %0" : "=r"(cr2));
    return cr2;
}
/**
 * @brief 设置cr3
 * x86-64架构
 * 
 * @param cr3 cr3寄存器值 
 */
ARCHITECTURE_BASED INLINE PUBLIC void __set_cr3(bits cr3) {
    asmv ("movq %0, %%cr3" : : "r"(cr3));
}
/**
 * @brief 获得cr3
 * x86-64架构
 * 
 * @return cr3寄存器值 
 */
ARCHITECTURE_BASED INLINE PUBLIC bits __get_cr3(void) {
    bits cr3;
    asmv ("movq %%cr3, %0" : "=r"(cr3));
    return cr3;
}
/**
 * @brief 设置cr4
 * x86-64架构
 * 
 * @param cr4 cr4寄存器值 
 */
ARCHITECTURE_BASED INLINE PUBLIC void __set_cr4(bits cr4) {
    asmv ("movq %0, %%cr4" : : "D"(cr4));
}
/**
 * @brief 获得cr4
 * x86-64架构
 * 
 * @return cr4寄存器值 
 */
ARCHITECTURE_BASED INLINE PUBLIC bits __get_cr4(void) {
    bits cr4;
    asmv ("movq %%cr4, %0" : "=r"(cr4));
    return cr4;
}

#ifndef LOADER32BIT
/**
 * @brief 设置msr
 * x86-64架构
 * 
 * @param msr MSR序号
 * @param val 值
 */
ARCHITECTURE_BASED INLINE PUBLIC void __set_msr(dword msr, qword val) {
    dword high32 = val >> 32;
    dword low32 = val & 0xFFFFFFFF;
    asmv ("\
movq %0, %%rcx \n\
movq %1, %%rdx \n\
movq %2, %%rax \n\
wrmsr \n\
" 
: : "r"(msr), "r"(high32), "r"(low32)
: "rcx", "rdx", "rax");
}
/**
 * @brief 获得msr
 * x86-64架构
 * 
 * @return msr寄存器值 
 */
ARCHITECTURE_BASED INLINE PUBLIC bits __get_msr(dword msr) {
    dword high32;
    dword low32;
    asmv ("\
movq %2, %%rcx \n\
rdmsr \n\
movq %%rdx, %0 \n\
movq %%rax, %1 \n\
" 
: "=r"(high32), "=r"(low32) : "r"(msr) 
: "rcx", "rdx", "rax");
    return ((qword)high32) << 32 | (low32 & 0xFFFFFFFF);
}
#else

/**
 * @brief 设置msr
 * x86架构
 * 
 * @param msr MSR序号
 * @param val 值
 */
ARCHITECTURE_BASED INLINE PUBLIC void __set_msr(dword msr, dword val) {
    asmv ("\
movl %0, %%ecx \n\
movl %1, %%edx \n\
xorl %%eax, %%eax \n\
wrmsr \n\
" 
: : "r"(msr), "r"(val)
: "ecx", "edx", "eax");
}
/**
 * @brief 获得msr
 * x86架构
 * 
 * @return msr寄存器值 
 */
ARCHITECTURE_BASED INLINE PUBLIC bits __get_msr(dword msr) {
    dword val;
    asmv ("\
movl %1, %%ecx \n\
rdmsr \n\
movl %%eax, %0 \n\
" 
: "=r"(val) : "r"(msr) 
: "ecx", "edx", "eax");
    return val;
}
#endif

/**
 * @brief 设置efer
 * x86-64架构
 * 
 * @param efer efer寄存器值 
 */
ARCHITECTURE_BASED INLINE PUBLIC void __set_efer(bits efer) {
    __set_msr(EFER_MSR, efer);
}
/**
 * @brief 获得efer
 * x86-64架构
 * 
 * @return efer寄存器值 
 */
ARCHITECTURE_BASED INLINE PUBLIC qword __get_efer(void) {
    return __get_msr(EFER_MSR);
}

/**
 * @brief 设置cr0
 * x86-64架构
 * 
 * @param cr0 cr0寄存器值 
 */
ARCHITECTURE_BASED INLINE void set_cr0(cr0_t cr0) {
    __set_cr0(*(bits *)&cr0);
}

/**
 * @brief 获得cr0
 * x86-64架构
 * 
 * @return cr0寄存器值 
 */
ARCHITECTURE_BASED INLINE cr0_t get_cr0(void) {
    bits cr0 = __get_cr0();
    return *(cr0_t *)&cr0;
}

/**
 * @brief 设置cr2
 * x86-64架构
 * 
 * @param cr2 cr2寄存器值 
 */
ARCHITECTURE_BASED INLINE void set_cr2(cr2_t cr2) {
    __set_cr2(*(bits *)&cr2);
}

/**
 * @brief 获得cr2
 * x86-64架构
 * 
 * @return cr2寄存器值 
 */
ARCHITECTURE_BASED INLINE cr2_t get_cr2(void) {
    bits cr2 = __get_cr2();
    return *(cr2_t *)&cr2;
}

/**
 * @brief 设置cr3
 * x86-64架构
 * 
 * @param cr3 cr3寄存器值 
 */
ARCHITECTURE_BASED INLINE void set_cr3(cr3_t cr3) {
    __set_cr3(*(bits *)&cr3);
}

/**
 * @brief 获得cr3
 * x86-64架构
 * 
 * @return cr3寄存器值 
 */
ARCHITECTURE_BASED INLINE cr3_t get_cr3(void) {
    bits cr3 = __get_cr3();
    return *(cr3_t *)&cr3;
}

/**
 * @brief 设置cr4
 * x86-64架构
 * 
 * @param cr4 cr4寄存器值 
 */
ARCHITECTURE_BASED INLINE void set_cr4(cr4_t cr4) {
    __set_cr4(*(bits *)&cr4);
}

/**
 * @brief 获得cr4
 * x86-64架构
 * 
 * @return cr4寄存器值 
 */
ARCHITECTURE_BASED INLINE cr4_t get_cr4(void) {
    bits cr4 = __get_cr4();
    return *(cr4_t *)&cr4;
}

/**
 * @brief 设置efer
 * x86-64架构
 * 
 * @param efer efer寄存器值 
 */
ARCHITECTURE_BASED INLINE void set_efer(efer_t efer) {
    __set_efer(*(bits *)&efer);
}

/**
 * @brief 获得efer
 * x86-64架构
 * 
 * @return efer寄存器值 
 */
ARCHITECTURE_BASED INLINE efer_t get_efer(void) {
    bits efer = __get_efer();
    return *(efer_t *)&efer;
}

/**
 * @brief 刷新CS
 * 
 * 需要自己实现
 * 
 * @param cs_sel CS选择子
 */
ARCHITECTURE_BASED ASSEMBLY void flush_cs(word cs_sel);