/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * main.c
 *
 * setup主函数
 *
 */



#include <debug/logging.h>

#include <assert.h>

#include <syscall/syscall.h>
#include <syscall/ipc.h>

#include <memory/malloc.h>

#include <tayhuang/services.h>
#include <tayhuang/msgpack.h>

#include <disk.h>
#include <fs/common.h>

PUBLIC void kmod_main(void) {
    set_logging_name("Setup");

    reg_irq(14);
    reg_irq(15);

    char name[256];
    void *buffer;

    fs_context context = NULL;

    while (true) {
        set_allow(INIT_SERVICE);

	int recv_ret;
        check_ipc();
        recv_ret = recv_msg(name).source != -1;
	assert(recv_ret);
        check_ipc();
        recv_ret = recv_msg(&buffer).source != -1;
	assert(recv_ret);

        if (context == NULL) {
            context = load_fs(DISK_SEL_IDE0_MASTER, 0);
        }

        linfo ("%s:%p", name, buffer);

        bool status = load_file(context, name, buffer);

	int send_ret = send_msg(MSG_NORMAL_IPC, &status, sizeof(bool), INIT_SERVICE);
	assert(send_ret);
    }

    terminate_fs (context);

    while (true);
}
