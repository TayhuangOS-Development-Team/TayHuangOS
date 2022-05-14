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
 * ipc.h
 * 
 * IPC
 * 
 */



#pragma once

int send_msg(void *msg, int dest, int len);
int recv_msg(void *msg, int source);
int recv_any_msg(void *msg);