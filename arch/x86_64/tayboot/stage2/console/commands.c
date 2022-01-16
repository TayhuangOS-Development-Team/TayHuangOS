// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/console/commands.c
 *
 * Real mode console commands are implemented here
 */



#include "commands.h"
#include <string.h>
#include "../printf.h"
#include "../scanf.h"
#include "../intcall.h"

PRIVATE void get_arg(char *arg) {
    char __arg[128];
    char *_arg = __arg;
    scanf ("%s", _arg);
    if (*_arg == '"') {
        _arg ++;
        while (*(_arg + strlen(_arg) - 1) != '"') {
            strcpy(arg, _arg);
            arg += strlen(_arg);
            *(arg ++) = getchar();
            scanf ("%s", _arg);
        }
        *(_arg + strlen(_arg) - 1) = '\0';
        strcpy(arg, _arg);
    }
    else {
        strcpy(arg, _arg);
        return;
    }
}

DEF_CONSOLE_CMD(echo) {
    char sentence[64];
    get_arg(sentence);
    printf ("%s", sentence);
    return 0;
}

DEF_CONSOLE_CMD(echoln) {
    char sentence[64];
    get_arg(sentence);
    printf ("%s\n", sentence);
    return 0;
}

DEF_CONSOLE_CMD(shutdown) {
    intargs_t args;
    reg_collect_t in_regs, out_regs;
    in_regs.eax = MKDWORD(0, 0x5307);
    in_regs.ebx = MKDWORD(0, MKWORD(0, 1));
    in_regs.ecx = MKDWORD(0, 0x0003);
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    args.int_no = 0x15; 
    intcall(&args);
    return -1;
}

DEF_CONSOLE_CMD(help) {
    printf ("help: show help\n");
    printf ("echo [sentence]: write sentence into screen\n");
    printf ("echoln [sentence]: write sentence and \\n into screen\n");
    printf ("shutdown: power off\n");
    return 0;
}