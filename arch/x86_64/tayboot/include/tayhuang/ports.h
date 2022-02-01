// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/include/tayhuang/ports.h
 *
 * 基础端口
 */



#pragma once

//PIC
#define M_PIC_CONTROL 0x20
#define M_PIC_DATA 0x21
#define S_PIC_CONTROL 0xA0
#define S_PIC_DATA 0xA1

//CMOS RAM
#define CMOS_RAM_ADDRESS 0x70 
#define CMOS_RAM_DATA 0x71

//键盘8042
#define KEYBOARD_8042_DATA0 0x60
#define KEYBOARD_8042_DATA1 0x61
#define KEYBOARD_8042_CONTROL 0x62
#define KEYBOARD_8255_INPUT 0x62
#define KEYBOARD_8255_CMD 0x63
#define KEYBOARD_8042_DATA2 0x64
#define KEYBOARD_8042_STATUS 0x64

//协处理器
#define CO_CPU_0 0xF0
#define CO_CPU_1 0xF1
#define CO_CPU_2 0xF2
#define CO_CPU_3 0xF3
#define CO_CPU_4 0xF4
#define CO_CPU_5 0xF5
#define CO_CPU_6 0xF6
#define CO_CPU_7 0xF7
#define CO_CPU_8 0xF8
#define CO_CPU_9 0xF9
#define CO_CPU_A 0xFA
#define CO_CPU_B 0xFB
#define CO_CPU_C 0xFC
#define CO_CPU_D 0xFD
#define CO_CPU_E 0xFE
#define CO_CPU_F 0xFF

//Port-A
#define PORT_A 0x92

//0号IDE接口
#define IDE0_DATA 0x1F0
#define IDE0_ERROR 0x1F1
#define IDE0_FEATURES 0x1F1
#define IDE0_SECTOR_COUNTER 0x1F2
#define IDE0_LBA_LOW 0x1F3
#define IDE0_LBA_MID 0x1F4
#define IDE0_LBA_HIGH 0x1F5
#define IDE0_DEVICE 0x1F6
#define IDE0_STATUS 0x1F7
#define IDE0_COMMAND 0x1F7
#define IDE0_ALTERNATE_STATUS 0x3F6
#define IDE0_DEVICE_CONTROL 0x3F6

//1号IDE接口
#define IDE1_DATA 0x170
#define IDE1_ERROR 0x171
#define IDE1_FEATURES 0x171
#define IDE1_SECTOR_COUNTER 0x172
#define IDE1_LBA_LOW 0x173
#define IDE1_LBA_MID 0x174
#define IDE1_LBA_HIGH 0x175
#define IDE1_DEVICE 0x176
#define IDE1_STATUS 0x177
#define IDE1_COMMAND 0x177
#define IDE1_ALTERNATE_STATUS 0x376
#define IDE1_DEVICE_CONTROL 0x376
