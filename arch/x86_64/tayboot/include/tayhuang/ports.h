// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/include/tayhuang/ports.h
 *
 * Basic ports
 */



#pragma once

#define M_8042A_CTL 0x20
#define M_8042A_CTLMASK 0x21
#define S_8042A_CTL 0xA0
#define S_8042A_CTLMASK 0xA1
#define CMOS_RAM_ADR 0x70 
#define CMOS_TAM_DAT 0x71
#define KEYBOARD_8042_DATA0 0x60
#define KEYBOARD_8042_DATA1 0x61
#define KEYBOARD_8042_CONTROL 0x62
#define KEYBOARD_8255_INPUT 0x62
#define KEYBOARD_8255_CMD 0x63
#define KEYBOARD_8042_DATA2 0x64
#define KEYBOARD_8042_STATUS 0x64
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
#define PORT_A 0x92