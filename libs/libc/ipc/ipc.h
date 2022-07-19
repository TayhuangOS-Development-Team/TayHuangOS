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
 * ipc.h
 * 
 * IPC
 * 
 */



#pragma once

//FIXME: 重构

//IPC相关函数
int send_msg(void *msg, int dest, int len, int tickout);
int recv_msg(void *msg, int source);
int recv_any_msg(void *msg);
int recv_any_msg_and_wait(void *msg);