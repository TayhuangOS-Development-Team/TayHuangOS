// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * include/tayhuang/paging.h
 *
 * 分页相关结构/宏
 */



#pragma once

#include <tayhuang/defs.h>

#define MEMUNIT_SZ (4096) //内存单元大小

#ifdef ARCH_x86_64

#define CALC_M(x) min(54, max((sizeof(dword) * 8 - leading_zeros(x - 1)), 32)) //根据内存大小计算M的值

typedef struct {
    bool p : 1;
    bool rw : 1;
    bool us : 1;
    bool pwt : 1;
    bool pcd : 1;
    bool a : 1;
    byte avl3 : 1;
    byte reserved2 : 1;
    byte avl2 : 4;
    qword address : 39;
    byte reserved : 1;
    word avl : 11;
    bool xd : 1;
} PML4E;

typedef struct {
    bool p : 1;
    bool rw : 1;
    bool us : 1;
    bool pwt : 1;
    bool pcd : 1;
    bool a : 1;
    byte avl3 : 1;
    byte reserved2 : 1;
    byte avl2 : 4;
    qword address : 39;
    byte reserved : 1;
    word avl : 11;
    bool xd : 1;
} PDPTE;

typedef struct {
    bool p : 1;
    bool rw : 1;
    bool us : 1;
    bool pwt : 1;
    bool pcd : 1;
    bool a : 1;
    byte avl3 : 1;
    byte reserved2 : 1;
    byte avl2 : 4;
    qword address : 39;
    byte reserved : 1;
    word avl : 11;
    bool xd : 1;
} PDE;

typedef struct {
    bool p : 1;
    bool rw : 1;
    bool us : 1;
    bool pwt : 1;
    bool pcd : 1;
    bool a : 1;
    byte d : 1;
    byte pat : 1;
    byte g : 1;
    byte avl2 : 3;
    qword address : 39;
    byte reserved : 1;
    word avl : 11;
    bool xd : 1;
} PTE;

typedef struct {
    PML4E *start;
    int entry_num;
    void *nxt;
} PML4;

typedef struct {
    PDPTE *start;
    int entry_num;
    void *nxt;
} PDPT;

typedef struct {
    PDE *start;
    int entry_num;
    void *nxt;
} PD;

typedef struct {
    PTE *start;
    int entry_num;
    void *nxt;
} PT;

#else

typedef struct {
    bool p : 1;
    byte res : 2;
    bool pwt : 1;
    bool pcd : 1;
    byte reserved2 : 4;
    byte avl : 3;
    dword address : 24;
    dword reserved : 28;
} PDPTE;

typedef struct {
    bool p : 1;
    byte res : 2;
    bool pwt : 1;
    bool pcd : 1;
    byte reserved2 : 4;
    byte avl : 3;
    dword address : 24;
    dword reserved : 28;
} PDE;

typedef struct {
    bool p : 1;
    bool rw : 1;
    bool us : 1;
    bool pwt : 1;
    bool pcd : 1;
    byte reserved2 : 4;
    byte avl : 3;
    dword address : 24;
    dword reserved : 28;
} PTE;

typedef struct {
    bool p : 1;
    byte res : 2;
    bool pwt : 1;
    bool pcd : 1;
    byte reserved2 : 4;
    byte avl : 3;
    dword address : 20;
} PDE32;

typedef struct {
    bool p : 1;
    bool rw : 1;
    bool us : 1;
    bool pwt : 1;
    bool pcd : 1;
    byte reserved2 : 4;
    byte avl : 3;
    dword address : 20;
} PTE32;

#endif

