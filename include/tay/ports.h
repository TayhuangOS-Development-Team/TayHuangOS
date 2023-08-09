/**
 * @file ports.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 端口表
 * @version alpha-1.0.0
 * @date 2023-04-30
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

/** 主PIC基址 */
#define M_PIC_BASE         (0x20)
/** 从PIC基址 */
#define S_PIC_BASE         (0xA0)
/** PIC控制端口偏移 */
#define PIC_CONTROL (0)
/** PIC数据端口偏移 */
#define PIC_DATA    (1)

//CMOS RAM
#define CMOS_RAM_BASE    (0x70)
#define CMOS_RAM_ADDRESS (CMOS_RAM_BASE + 0)
#define CMOS_RAM_DATA    (CMOS_RAM_BASE + 1)

//键盘8042
#define KEYBOARD_8042_BASE    (0x60)
#define KEYBOARD_8042_DATA0   (KEYBOARD_8042_BASE + 0)
#define KEYBOARD_8042_DATA1   (KEYBOARD_8042_BASE + 1)
#define KEYBOARD_8255_CONTROL (KEYBOARD_8042_BASE + 2)
#define KEYBOARD_8255_INPUT   (KEYBOARD_8042_BASE + 2)
#define KEYBOARD_8255_CMD     (KEYBOARD_8042_BASE + 3)
#define KEYBOARD_8042_DATA2   (KEYBOARD_8042_BASE + 4)
#define KEYBOARD_8042_CONTROL (KEYBOARD_8042_BASE + 4)
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

/** Port-A */
#define PORT_A (0x92)

/** 0号IDE通道基址 */
#define IDE0_BASE             (0x1F0)
/** 0号IDE通道基址2 */
#define IDE0_BASE2            (0x3F6)

/** 1号IDE通道基址 */
#define IDE1_BASE             (0x170)
/** 1号IDE通道基址2 */
#define IDE1_BASE2            (0x376)

/** IDE通道数据端口偏移 */
#define IDE_DATA             (0)
/** IDE通道错误端口偏移 */
#define IDE_ERROR            (1)
/** IDE通道特征端口偏移 */
#define IDE_FEATURES         (1)
/** IDE通道扇区计数端口偏移 */
#define IDE_SECTCNT  (2)
/** IDE通道LBA(低字节)端口偏移 */
#define IDE_LBA_LOW          (3)
/** IDE通道LBA(中字节)端口偏移 */
#define IDE_LBA_MID          (4)
/** IDE通道LBA(高字节)端口偏移 */
#define IDE_LBA_HIGH         (5)
/** IDE通道设备端口偏移 */
#define IDE_DEVICE           (6)
/** IDE通道状态端口偏移 */
#define IDE_STATUS           (7)
/** IDE通道命令端口偏移 */
#define IDE_COMMAND          (7)
/** IDE通道替换状态端口偏移 */
#define IDE_ALTERNATE_STATUS (0)
/** IDE通道设备控制端口偏移 */
#define IDE_DEVICE_CONTROL   (0)

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

//Delay Port
#define DELAY_PORT (0x80)