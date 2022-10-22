/**
 * @file paging.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 分页
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

#define MEMUNIT_SZ (4096) //内存单元大小

#define CALC_M(x) min(54, max((sizeof(dword) * 8 - leading_zeros(x - 1)), 32)) //根据内存大小计算M的值

//页项结构
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