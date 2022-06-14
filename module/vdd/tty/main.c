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
 * main.c
 *
 * tty 主函数
 *
 */



#include <debug/logging.h>
#include <ipc/ipc.h>
#include <tayhuang/services.h>
#include <memory/malloc.h>
#include "tty.h"
#include "cmd.h"

#define TTY_NUM (3)
tty_struct ttys[TTY_NUM];

enum {
    MODE_ANY = 0,
    MODE_TEXT = 1,
    MODE_GRAPHIC = 2
};

#define MKCMD(mode, code) ((((qword)(mode & 0xF)) << 28) | (((qword)(code)) & 0x0FFFFFFF))

#define __TEXT_WRITE_CHAR (0)
#define TEXT_WRITE_CHAR MKCMD(MODE_TEXT, __TEXT_WRITE_CHAR)

#define __TEXT_WRITE_CHARS (1)
#define TEXT_WRITE_CHARS MKCMD(MODE_TEXT, __TEXT_WRITE_CHARS)

#define __CLEAR_SCREEN (2)
#define CLEAR_SCREEN MKCMD(MODE_ANY, __CLEAR_SCREEN)

#define __TEXT_SET_START_ADDR (3)
#define TEXT_SET_START_ADDR MKCMD(MODE_TEXT, __TEXT_SET_START_ADDR)

#define SCREEN_SIZE (80 * 68 * 2)

void write_str(int ttyid, tty_struct *tty, int num_characters, qword *str) {
    static qword command[600];
    
    command[0] = TEXT_WRITE_CHARS;
    command[1] = num_characters;
    command[2] = ttyid * SCREEN_SIZE + tty->offset;

    for (int i = 0 ; i < num_characters ; i ++) {
        command[i * 4 + 3] = str[i * 2];
        command[i * 4 + 4] = str[i * 2 + 1];
        command[i * 4 + 5] = tty->pos_x;
        command[i * 4 + 6] = tty->pos_y;

        tty->pos_x ++;
        tty->pos_y += tty->pos_x / 80;
        tty->pos_x %= 80;
    }

    send_msg(command, VIDEO_DRIVER_SERVICE, sizeof(command), 20);
}

void deal_cmd(int caller, qword cmd, qword *param) {
    switch (cmd)
    {
    case TTY_PUTCHAR: {
        int ttyid = param[0];
        tty_struct *tty = &ttys[ttyid];

        qword command[6] = {TEXT_WRITE_CHAR, ttyid * SCREEN_SIZE + tty->offset, param[1], param[2], tty->pos_x, tty->pos_y};
        send_msg(command, VIDEO_DRIVER_SERVICE, sizeof(command), 20);
        
        tty->pos_x ++;
        tty->pos_y += tty->pos_x / 80;
        tty->pos_x %= 80;
        break;
    }
    case TTY_WRITE_STR: {
        int ttyid = param[0];
        tty_struct *tty = &ttys[ttyid];
        int num_characters = param[1];

        write_str(ttyid, tty, num_characters, &param[2]);
        break;
    }
    case TTY_CLEAR_SCREEN: {
        int ttyid = param[0];
        tty_struct *tty = &ttys[ttyid];

        qword command[6] = {CLEAR_SCREEN, ttyid * SCREEN_SIZE, SCREEN_SIZE};
        send_msg(command, VIDEO_DRIVER_SERVICE, sizeof(command), 20);

        tty->pos_x = tty->pos_y = 0;
        break;
    }
    case TTY_GETCHAR: {
        break;
    }
    case TTY_GETCHARS: {
        break;
    }
    case TTY_GETPOSX: {
        int ttyid = param[0];
        tty_struct *tty = &ttys[ttyid];

        int pos_x = tty->pos_x;

        send_msg(&pos_x, caller, sizeof(pos_x), 20);
        break;
    }
    case TTY_GETPOSY: {
        int ttyid = param[0];
        tty_struct *tty = &ttys[ttyid];

        int pos_y = tty->pos_y;

        send_msg(&pos_y, caller, sizeof(pos_y), 20);
        break;
    }
    case TTY_SETPOS: {
        int ttyid = param[0];
        tty_struct *tty = &ttys[ttyid];

        tty->pos_x = param[1];
        tty->pos_y = param[2];
        break;
    }
    case TTY_SETACTIVE: {
        int ttyid = param[0];

        qword command[] = {TEXT_SET_START_ADDR, ttyid * SCREEN_SIZE / 2};
        send_msg(command, VIDEO_DRIVER_SERVICE, sizeof(command), 20);
        break;
    }
    case TTY_SETSCROLLLINE: {
        int ttyid = param[0];
        tty_struct *tty = &ttys[ttyid];

        tty->scroll_line = param[1];
        break;
    }
    case TTY_GETSCROLLLINE: {
        int ttyid = param[0];
        tty_struct *tty = &ttys[ttyid];

        int scroll_line = tty->scroll_line;

        send_msg(&scroll_line, caller, sizeof(scroll_line), 20);
        break;
    }
    default: {
        lwarn ("TTY Driver Received an unknown command!");
        break;
    }
    }
}

void kmod_main(void) {
    set_logging_name("TTY");

    linfo ("TTY Here!");

    init_heap();

    for (int i = 0 ; i < TTY_NUM ; i ++) {
        ttys[i].pos_x = ttys[i].pos_y = ttys[i].offset = ttys[i].scroll_line = 0;
    }

    qword *command = malloc(300 * sizeof(qword));

    while (true) {
        int caller = 0;

        while ((caller = recv_any_msg(command)) == -1);

        deal_cmd(caller, command[0], &command[1]);
    }

    free(command);
    while(true);
}