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
#include "keymap.h"

#include <display/printk.h>
#include <tayhuang/io.h>
#include <kheap.h>
#include <syscall/syscall.h>

#define BUFFER_LEN (256) //缓冲区大小

PRIVATE struct {
    int tail;
    int head;
    int cnt;
    char buffer[BUFFER_LEN];
} kb_buffer, char_buffer;

PUBLIC void init_keyboard(void) { //初始化键盘
    kb_buffer.tail = kb_buffer.head = kb_buffer.cnt = 0;
    memset(kb_buffer.buffer, 0, sizeof(kb_buffer.buffer));

    char_buffer.tail = char_buffer.head = char_buffer.cnt = 0;
    memset(char_buffer.buffer, 0, sizeof(char_buffer.buffer));
}

void after_syscall(struct intterup_args *regs);
PRIVATE volatile int newlined_num = 0; //剩余新行的数量

PUBLIC char getchar(void) { //获得一个处理过的字符
    while (newlined_num <= 0);
    char ch = char_buffer.buffer[char_buffer.tail];
    char_buffer.tail = (char_buffer.tail + 1) % BUFFER_LEN;
    char_buffer.cnt --;
    if (ch == '\n')
        newlined_num --;
    return ch;
}

PRIVATE void __kb_putchar(char ch) { //放入处理过的字符
    putchar(ch);
    flush_to_screen();
    char_buffer.buffer[char_buffer.head] = ch;
    char_buffer.head = (char_buffer.head + 1) % BUFFER_LEN;
    char_buffer.cnt ++;
    if (ch == '\n')
        newlined_num ++;
}

//状态
struct {
    bool lshift_pushing : 1;
    bool rshift_pushing : 1;
    bool lctrl_pushing : 1;
    bool rctrl_pushing : 1;
    bool lalt_pushing : 1;
    bool ralt_pushing : 1;
    bool capslock_pushing : 1;
    bool numlock_pushing : 1;
    bool scrolllock_pushing : 1;
    bool E0_CODE : 1;
    bool E1_CODE : 1;
} keyboard_states;

PUBLIC void deal_key(void) {
    if (kb_buffer.cnt <= 0) //无键可取
        return;

    byte code = kb_buffer.buffer[kb_buffer.tail];
    kb_buffer.tail = (kb_buffer.tail + 1) % BUFFER_LEN;
    kb_buffer.cnt --; //取键

    dis_int(); //关终端 (以下是一个原子操作)
    if (code == 0xE0) { //双字节键
        keyboard_states.E0_CODE = true;
    }
    else if (code == 0xE1) { //多字节键
        //省略
    }
    else { //单字节键
        short rawcode = KEYMAP[code & 0x7F][0]; //未经任何加工所获得的对照码（生码）
        if (code < 0x80) { //事makecode
            if (keyboard_states.E0_CODE) { //事双字节code
                keyboard_states.E0_CODE = false;
                rawcode = KEYMAP[code & 0x7F][2]; //改变生码
            }
            if (rawcode <= 255) {
                if (rawcode >= 'a' && rawcode <= 'z') { //事字母
                    __kb_putchar(KEYMAP[code & 0x7F][keyboard_states.capslock_pushing | keyboard_states.lshift_pushing | keyboard_states.rshift_pushing]); //要额外判断Capslock
                }
                else {
                    __kb_putchar(KEYMAP[code & 0x7F][keyboard_states.lshift_pushing | keyboard_states.rshift_pushing]);
                }
            }
            else if ((rawcode >= PAD_HOME && rawcode <= PAD_DOT) || (rawcode >= HOME && rawcode <= KEY_DELETE)) { //事小键盘
                __kb_putchar(KEYMAP[code & 0x7F][keyboard_states.numlock_pushing | keyboard_states.lshift_pushing | keyboard_states.rshift_pushing]);
            }
            else if (rawcode == ENTER) { //事回车
                __kb_putchar('\n');
            }
            else if (rawcode == LSHIFT) { //功能键
                keyboard_states.lshift_pushing = true;
            }
            else if (rawcode == RSHIFT) {
                keyboard_states.rshift_pushing = true;
            }
            else if (rawcode == LCTRL) {
                keyboard_states.lctrl_pushing = true;
            }
            else if (rawcode == RCTRL) {
                keyboard_states.rctrl_pushing = true;
            }
            else if (rawcode == LALT) {
                keyboard_states.lalt_pushing = true;
            }
            else if (rawcode == RALT) {
                keyboard_states.ralt_pushing = true;
            }
            else if (rawcode == CAPSLOCK) {
                keyboard_states.capslock_pushing = ! keyboard_states.capslock_pushing;
            }
            else if (rawcode == NUMLOCK) {
                keyboard_states.numlock_pushing = ! keyboard_states.numlock_pushing;
            }
            else if (rawcode == SCROLL_LOCK) {
                keyboard_states.scrolllock_pushing = ! keyboard_states.scrolllock_pushing;
            }
            else if (rawcode == BACKSPACE) { //事退格
                if (char_buffer.cnt > 0) {
                    char_buffer.cnt --;
                    char_buffer.head = (char_buffer.head == 0) ? (BUFFER_LEN - 1) : (char_buffer.head - 1);
                    putchar('\b');
                }
            }
        }
        if (code >= 0x80) { //事breakcode
            if (keyboard_states.E0_CODE) { //事双字节code
                keyboard_states.E0_CODE = false;
            }
            if (rawcode == LSHIFT) { //功能键
                keyboard_states.lshift_pushing = false;
            }
            else if (rawcode == RSHIFT) {
                keyboard_states.rshift_pushing = false;
            }
            else if (rawcode == LCTRL) {
                keyboard_states.lctrl_pushing = false;
            }
            else if (rawcode == RCTRL) {
                keyboard_states.rctrl_pushing = false;
            }
            else if (rawcode == LALT) {
                keyboard_states.lalt_pushing = false;
            }
            else if (rawcode == RALT) {
                keyboard_states.ralt_pushing = false;
            }
        }
    }
    en_int(); //结束原子操作

}

PUBLIC short keyboard_int_handler(int irq, struct intterup_args *regs, bool entered_handler) {
    kb_buffer.buffer[kb_buffer.head] = inb(0x60);
    kb_buffer.head = (kb_buffer.head + 1) % BUFFER_LEN;
    kb_buffer.cnt ++; //键入列

    if (! entered_handler) //不是嵌套中断
        after_syscall(regs);
    return 0;
}

PUBLIC void keyboard_handler(void) {
    while (true) {
        deal_key(); //处理按键
    }
}

PUBLIC void keyboard_api_process(void) {
    while (true) {
        qword pack[20];
        int caller = 0;
        while ((caller = receive_any_msg(pack)) == -1);
        if (pack[0] == 0) {
        }
    }
}