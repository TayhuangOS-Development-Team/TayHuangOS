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
 * MM主函数
 *
 */



#include <syscall/syscall.h>
#include <debug/logging.h>
#include <ipc/ipc.h>
#include <tool/tostring.h>
#include <malloc.h>

#define MM_INIT_HEAP (0)
#define MM_MALLOC (1)
#define MM_FREE (2)

void kmod_main(void) {
    linfo ("MM!");

    while (true) {
        qword pack[20];
        int caller = 0;
        while ((caller = recv_any_msg(pack)) == -1);
        int cmdid = pack[0];
        switch (cmdid)
        {
        case MM_INIT_HEAP: {
            bool status = theap_init(caller);
            send_msg (&status, caller, sizeof(status), 20);
            break;
        }
        case MM_MALLOC: {
            void *addr = tmalloc(pack[1], caller);
            send_msg(&addr, caller, sizeof(addr), 20);
            break;
        }
        case MM_FREE: {
            tfree ((void*)pack[1], caller);
            break;
        }
        default: {
            lerror("MM received an unknown command");
            break;
        }
        }
    }
}