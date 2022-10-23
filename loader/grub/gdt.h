#pragma once

#include <tayhuang/attributes.h>
#include <tayhuang/descs.h>

#define GDT_SIZE (16)

//GDT表
EXTERN PUBLIC struct desc_struct GDT[GDT_SIZE];
//指针
EXTERN PUBLIC struct gdt_ptr gdtr;

//初始化函数
PUBLIC void init_gdt(void);