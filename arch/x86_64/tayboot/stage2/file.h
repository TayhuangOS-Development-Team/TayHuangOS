// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/file.h
 *
 * File functions are declared here
 */

#pragma once

#include "header.h"

typedef struct {
    addr_t file_buffer;
    addr_t file_info;
    int rdpos;
    int wrpos;
}FILE;

PUBLIC void load_file(FILE *file, const char* name);
PUBLIC void set_read_pos(FILE *file, int pos);
PUBLIC int get_read_pos(FILE *file);
PUBLIC void terminate_file(FILE *file);