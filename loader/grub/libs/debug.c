/**
 * @file debug.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 调试
 * @version alpha-1.0.0
 * @date 2023-04-30
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#include <libs/debug.h>
#include <tay/ports.h>
#include <tay/io.h>

void InitSerial(void) {
    outb(SERIAL_INT_VALID, 0); //禁用COM中断
    outb(SERIAL_CONTROL, 0x80); //启用DLAB
    outb(SERIAL_SEND, 0x03); //设置比特波除数(低)
    outb(SERIAL_INT_VALID, 0x00); //设置比特波除数(高)
    outb(SERIAL_CONTROL, 0x03); //无奇偶性 1停止位
    outb(SERIAL_INT_ID, 0xC7); //FIFO(size = 14)
    outb(SERIAL_MODEM_CONTROL, 0x0B);
    outb(SERIAL_MODEM_CONTROL, 0x1E);
    outb(SERIAL_SEND, 0xAE);
    outb(SERIAL_MODEM_CONTROL, 0x0F);
}

void WriteSerialChar(char ch) {
    while ((inb(SERIAL_STATUS) & 0x20) == 0);
    outb(SERIAL_SEND, ch);
}

void WriteSerialStr(const char *str) {
    // asm volatile ("cli");
    while (*str != '\0') {
        WriteSerialChar(*str);
        str ++;
    }
    // asm volatile ("sti");
}