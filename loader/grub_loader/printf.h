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
 * printf.h
 *
 * printf
 *
 */



#pragma once

#define VIDEO_ADDRESS (0xB8000)

void write_char(char ch, int color, int posx, int posy);
void write_str(const char *str, int color, int posx, int posy);