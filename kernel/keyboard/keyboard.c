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
 * keyboard.c
 *
 * 键盘
 *
 */

#include "keyboard.h"
#include "../display/printk.h"
#include <tayhuang/io.h>
#include "../kheap.h"
#include "keymap.h"

#define BUFFER_LEN (256)

PRIVATE struct {
    int tail;
    int head;
    int cnt;
    char buffer[BUFFER_LEN];
} kb_buffer, char_buffer;

PUBLIC void init_keyboard(void) {
    kb_buffer.tail = kb_buffer.head = kb_buffer.cnt = 0;
    memset(kb_buffer.buffer, 0, sizeof(kb_buffer.buffer));

    char_buffer.tail = char_buffer.head = char_buffer.cnt = 0;
    memset(char_buffer.buffer, 0, sizeof(char_buffer.buffer));
}

void after_syscall(struct intterup_args *regs);

PRIVATE volatile bool lshift_pushing = false;
PRIVATE volatile bool rshift_pushing = false;
PRIVATE volatile int newlined_num = 0;

PUBLIC char getchar(void) {
    while (newlined_num <= 0);
    char ch = char_buffer.buffer[char_buffer.tail];
    char_buffer.tail = (char_buffer.tail + 1) % BUFFER_LEN;
    char_buffer.cnt --;
    if (ch == '\n')
        newlined_num --;
    return ch;
}

PRIVATE void __kb_putchar(char ch) {
    putchar(ch);
    flush_to_screen();
    char_buffer.buffer[char_buffer.head] = ch;
    char_buffer.head = (char_buffer.head + 1) % BUFFER_LEN;
    char_buffer.cnt ++;
    if (ch == '\n')
        newlined_num ++;
}

PUBLIC void deal_key(void) {
    if (kb_buffer.cnt <= 0)
        return;

    byte code = kb_buffer.buffer[kb_buffer.tail];
    kb_buffer.tail = (kb_buffer.tail + 1) % BUFFER_LEN;
    kb_buffer.cnt --;

    dis_int();
    if (code == 0xE0) {

    }
    else if (code == 0xE1) {

    }
    if (code < 0x80) {
        if (KEYMAP[code & 0x7F][0] <= 255) {
            __kb_putchar(KEYMAP[code & 0x7F][lshift_pushing | rshift_pushing]);
        }
        else if (KEYMAP[code & 0x7F][0] == ENTER) {
            __kb_putchar('\n');
        }
        else if (KEYMAP[code & 0x7F][0] == LSHIFT) {
            lshift_pushing = true;
        }
        else if (KEYMAP[code & 0x7F][0] == RSHIFT) {
            rshift_pushing = true;
        }
        else if (KEYMAP[code & 0x7F][0] == BACKSPACE) {
            if (char_buffer.cnt > 0) {
                char_buffer.cnt --;
                char_buffer.head = (char_buffer.head == 0) ? (BUFFER_LEN - 1) : (char_buffer.head - 1);
                putchar('\b');
            }
        }
    }
    if (code >= 0x80) {
        if (KEYMAP[code & 0x7F][0] == LSHIFT) {
            lshift_pushing = false;
        }
        else if (KEYMAP[code & 0x7F][0] == RSHIFT) {
            rshift_pushing = false;
        }
    }
    en_int();

}

PUBLIC short keyboard_int_handler(int irq, struct intterup_args *regs, bool entered_handler) {
    kb_buffer.buffer[kb_buffer.head] = inb(0x60);
    kb_buffer.head = (kb_buffer.head + 1) % BUFFER_LEN;
    kb_buffer.cnt ++;

    if (! entered_handler)
        after_syscall(regs);
    return 0;
}