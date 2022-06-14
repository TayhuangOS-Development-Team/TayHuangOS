/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * ports.h
 *
 * 基础端口
 *
 */



#pragma once

//PIC
#define M_PIC_BASE    (0x20)
#define S_PIC_BASE    (0xA0)
#define M_PIC_CONTROL (M_PIC_BASE + 0)
#define M_PIC_DATA    (M_PIC_BASE + 1)
#define S_PIC_CONTROL (S_PIC_BASE + 0)
#define S_PIC_DATA    (S_PIC_BASE + 1)

//CMOS RAM
#define CMOS_RAM_BASE    (0x70)
#define CMOS_RAM_ADDRESS (CMOS_RAM_BASE + 0)
#define CMOS_RAM_DATA    (CMOS_RAM_BASE + 1)

//键盘8042
#define KEYBOARD_8042_BASE    (0x60)
#define KEYBOARD_8042_DATA0   (KEYBOARD_8042_BASE + 0)
#define KEYBOARD_8042_DATA1   (KEYBOARD_8042_BASE + 1)
#define KEYBOARD_8042_CONTROL (KEYBOARD_8042_BASE + 2)
#define KEYBOARD_8255_INPUT   (KEYBOARD_8042_BASE + 2)
#define KEYBOARD_8255_CMD     (KEYBOARD_8042_BASE + 3)
#define KEYBOARD_8042_DATA2   (KEYBOARD_8042_BASE + 4)
#define KEYBOARD_8042_STATUS  (KEYBOARD_8042_BASE + 4)

//协处理器
#define CO_CPU_BASE  (0xF0)
#define CO_CPU_0     (CO_CPU_BASE + 0x0)
#define CO_CPU_1     (CO_CPU_BASE + 0x1)
#define CO_CPU_2     (CO_CPU_BASE + 0x2)
#define CO_CPU_3     (CO_CPU_BASE + 0x3)
#define CO_CPU_4     (CO_CPU_BASE + 0x4)
#define CO_CPU_5     (CO_CPU_BASE + 0x5)
#define CO_CPU_6     (CO_CPU_BASE + 0x6)
#define CO_CPU_7     (CO_CPU_BASE + 0x7)
#define CO_CPU_8     (CO_CPU_BASE + 0x8)
#define CO_CPU_9     (CO_CPU_BASE + 0x9)
#define CO_CPU_A     (CO_CPU_BASE + 0xA)
#define CO_CPU_B     (CO_CPU_BASE + 0xB)
#define CO_CPU_C     (CO_CPU_BASE + 0xC)
#define CO_CPU_D     (CO_CPU_BASE + 0xD)
#define CO_CPU_E     (CO_CPU_BASE + 0xE)
#define CO_CPU_F     (CO_CPU_BASE + 0xF)

//Port-A
#define PORT_A (0x92)

//0号IDE通道
#define IDE0_BASE             (0x1F0)
#define IDE0_BASE2            (0x3F6)
#define IDE0_DATA             (IDE0_BASE + 0)
#define IDE0_ERROR            (IDE0_BASE + 1)
#define IDE0_FEATURES         (IDE0_BASE + 1)
#define IDE0_SECTOR_COUNTER   (IDE0_BASE + 2)
#define IDE0_LBA_LOW          (IDE0_BASE + 3)
#define IDE0_LBA_MID          (IDE0_BASE + 4)
#define IDE0_LBA_HIGH         (IDE0_BASE + 5)
#define IDE0_DEVICE           (IDE0_BASE + 6)
#define IDE0_STATUS           (IDE0_BASE + 7)
#define IDE0_COMMAND          (IDE0_BASE + 7)
#define IDE0_ALTERNATE_STATUS (IDE0_BASE2 + 0)
#define IDE0_DEVICE_CONTROL   (IDE0_BASE2 + 0)

//1号IDE通道
#define IDE1_BASE             (0x170)
#define IDE1_BASE2            (0x376)
#define IDE1_DATA             (IDE1_BASE + 0)
#define IDE1_ERROR            (IDE1_BASE + 1)
#define IDE1_FEATURES         (IDE1_BASE + 1)
#define IDE1_SECTOR_COUNTER   (IDE1_BASE + 2)
#define IDE1_LBA_LOW          (IDE1_BASE + 3)
#define IDE1_LBA_MID          (IDE1_BASE + 4)
#define IDE1_LBA_HIGH         (IDE1_BASE + 5)
#define IDE1_DEVICE           (IDE1_BASE + 6)
#define IDE1_STATUS           (IDE1_BASE + 7)
#define IDE1_COMMAND          (IDE1_BASE + 7)
#define IDE1_ALTERNATE_STATUS (IDE1_BASE2 + 0)
#define IDE1_DEVICE_CONTROL   (IDE1_BASE2 + 0)

//PIT
#define PIT_BASE     (0x40)
#define PIT_CHANNEL0 (PIT_BASE + 0)
#define PIT_CHANNEL1 (PIT_BASE + 1)
#define PIT_CHANNEL2 (PIT_BASE + 2)
#define PIT_COMMAND  (PIT_BASE + 3)

//SERIAL
#define SERIAL_BASE          (0x3F8)
#define SERIAL_SEND          (SERIAL_BASE + 0)
#define SERIAL_KEEP          (SERIAL_BASE + 0)
#define SERIAL_INT_VALID     (SERIAL_BASE + 1)
#define SERIAL_INT_ID        (SERIAL_BASE + 2)
#define SERIAL_CONTROL       (SERIAL_BASE + 3)
#define SERIAL_MODEM_CONTROL (SERIAL_BASE + 4)
#define SERIAL_STATUS        (SERIAL_BASE + 5)
#define SERIAL_MODEM_STATUS  (SERIAL_BASE + 6)

//CRT Controller
#define CRTC_BASE (0x3D4)
#define CRTC_ADDR (CRTC_BASE + 0)
#define CRTC_DATA (CRTC_BASE + 1)

//CRT Registers
#define CRTC_START_ADDR_H (0x0C)
#define CRTC_START_ADDR_L (0x0D)
#define CRTC_CURSOR_LOCATION_H (0x0E)
#define CRTC_CURSOR_LOCATION_L (0x0F)