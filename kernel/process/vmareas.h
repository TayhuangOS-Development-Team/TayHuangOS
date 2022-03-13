/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * vmareas.h
 *
 * 虚空间平衡树
 *
 */



#pragma once

#include <tayhuang/defs.h>

enum {
    CONTAINER_MEMORY,
    CONTAINER_DISK,
    CONTAINER_UNKNOWN
};

typedef struct _vm_area {
    int height;
    struct _vm_area *left;
    struct _vm_area *right;

    void *vm_start; //虚地址范围
    void *vm_end;

    struct {
        byte container_type : 4;
        word container_info : 12;
    } container_info;

    struct {
        bool present : 1; //是否在内存中
        bool read : 1; //是否能读
        bool write : 1; //是否能写
        bool execute : 1; //是否可执行
        bool shared : 1; //是否为共享内存
        byte reserved : 3;
    } attribute;

    struct {
        bool system : 1; //系统
        bool shared_read : 1; //共享读
        bool shared_write : 1; //共享写
        bool shared_execute : 1; //共享执行
        bool global_read : 1; //全局读
        bool global_write : 1; //全局写
        bool global_execute : 1; //全局执行
        byte reserved : 1;
    } protection;
} vm_area;

#define VM_ATTR_READ (0)
#define VM_ATTR_WRITE (1)
#define VM_ATTR_EXECUTE (2)
#define VM_ATTR_PRESENT (4)
#define VM_ATTR_SHARED (8)
#define VM_ATTR_SYSTEM (16)

PUBLIC vm_area *create_vmarea(void);
PUBLIC vm_area *vmarea_insert(vm_area *node, void *vm_start, void *vm_end,
    byte container_type, word container_info, byte shared_protection, byte global_protection, byte attribute);
PUBLIC vm_area *vmarea_delete(vm_area *node, void *vm_start, void *vm_end);