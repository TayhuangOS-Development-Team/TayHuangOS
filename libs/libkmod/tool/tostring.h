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
 * tostring.h
 *
 * xx转字符串
 *
 */



#pragma once

char *itoa(int val, char *buffer, int base);
char *uitoa(unsigned int val, char *buffer, int base);
char *lltoa(long long val, char *buffer, int base);
char *ulltoa(unsigned long long val, char *buffer, int base);
char *ftoa(float val, char *buffer, int round);
char *dtoa(double val, char *buffer, int round);
char *ftoea(float val, char *buffer, unsigned char upper_e);
char *dtoea(double val, char *buffer, unsigned char upper_e);