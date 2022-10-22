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


typedef struct {
    bool pe : 1;
    bool mp : 1;
    bool em : 1;
    bool ts : 1;
    bool et : 1;
    bool ne : 1;
    word reserved3 : 10;
    bool wp : 1;
    byte reserved2 : 1;
    bool am : 1;
    word reserved : 10;
    bool nw : 1;
    bool cd : 1;
    bool pg : 1;
} cr0_t; //CR0

typedef struct {
#ifdef ARCH_x86_64
    dword pfla;
#else
    qword pfla;
#endif
} cr2_t; //CR2

typedef union cr3_t {
    struct {
        byte reserved2 : 3;
        bool pwt : 1;
        bool pcd : 1;
        byte reserved3 : 7;
        qword _page_entry : 39;
        word reserved : 13;
    };
    qword page_entry;
} cr3_t; //CR3

typedef struct {
    bool vme : 1;
    bool pvi : 1;
    bool tsd : 1;
    bool de : 1;
    bool pse : 1;
    bool pae : 1;
    bool mce : 1;
    bool pge : 1;
    bool pce : 1;
    bool osfxsr : 1;
    bool osxmmexcpt : 1;
    bool umip : 1;;
    byte reserved : 1;
    bool cmxe : 1;
    byte reserved1 : 1;
    bool smxe : 1;
    bool fsgsbase : 1;
    bool pcide : 1;
    bool osxsave : 1;
    bool smep : 1;
    bool smap : 1;
    bool pke : 1;
    bool cet : 1;
    bool pks : 1;
    word reserved3 : 8;
} cr4_t; //CR4

typedef struct {
    bool sce : 1;
    byte reserved2 : 7;
    bool lme : 1;
    bool lma : 1;
    bool nxe : 1;
    bool svme : 1;
    bool lmsle : 1;
    bool ffxsr : 1;
    bool tce : 1;
    qword reserved : 16;
} efer_t; //EFER

//CR操作
IMPL_BY_USERS PUBLIC void __set_cr0(int cr0);
IMPL_BY_USERS PUBLIC int __get_cr0(void);
IMPL_BY_USERS PUBLIC void __set_cr2(int cr2);
IMPL_BY_USERS PUBLIC int __get_cr2(void);
IMPL_BY_USERS PUBLIC void __set_cr3(int cr3);
IMPL_BY_USERS PUBLIC int __get_cr3(void);
IMPL_BY_USERS PUBLIC void __set_cr4(int cr4);
IMPL_BY_USERS PUBLIC int __get_cr4(void);
IMPL_BY_USERS PUBLIC void __set_efer(int efer);
IMPL_BY_USERS PUBLIC int __get_efer(void);

//CR操作
INLINE void set_cr0(cr0_t cr0) {
    __set_cr0(*(int *)&cr0);
}

INLINE cr0_t get_cr0(void) {
    int cr0 = __get_cr0();
    return *(cr0_t *)&cr0;
}

INLINE void set_cr2(cr2_t cr2) {
    __set_cr2(*(int *)&cr2);
}

INLINE cr2_t get_cr2(void) {
    int cr2 = __get_cr2();
    return *(cr2_t *)&cr2;
}

INLINE void set_cr3(cr3_t cr3) {
    __set_cr3(*(int *)&cr3);
}

INLINE cr3_t get_cr3(void) {
    int cr3 = __get_cr3();
    return *(cr3_t *)&cr3;
}

INLINE void set_cr4(cr4_t cr4) {
    __set_cr4(*(int *)&cr4);
}

INLINE cr4_t get_cr4(void) {
    int cr4 = __get_cr4();
    return *(cr4_t *)&cr4;
}

INLINE void set_efer(efer_t efer) {
    __set_efer(*(int *)&efer);
}

INLINE efer_t get_efer(void) {
    int efer = __get_efer();
    return *(efer_t *)&efer;
}
