/* SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * control_registers.h
 *
 * 控制寄存器
 *
 */



#pragma once

#include <tayboot/tay_defs.h>

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

typedef struct {
    byte reserved2 : 3; 
    bool pwt : 1; 
    bool pcd : 1; 
    byte reserved3 : 7; 
    qword page_entry : 39;
    word reserved : 13;
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
    qword reserved : 48;
} efer_t; //EFER