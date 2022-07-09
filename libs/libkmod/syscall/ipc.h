/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong
 * 
 * ipc.h
 * 
 * IPC
 * 
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC bool send_msg(void *msg, qword size, int dst);
PUBLIC void check_ipc(void);
PUBLIC int recv_msg(void *msg);
PUBLIC void set_mailbuffer(void *buffer, int size);