// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/disk.h
 *
 * Disk functions are declared here
 */



#pragma once

#include "header.h"

void readsector(dword LDA_LOW, dword LDA_HIGH, word segment, word offset);
void initfsinfo(void);
void printfsinfo(void);
short getfatentry(short clus);
short findfile(const char *filename); //the length of file name must be 11 bytes
void loadfile(const char *filename, word segment, word offset); //the length of file name must be 11 bytes