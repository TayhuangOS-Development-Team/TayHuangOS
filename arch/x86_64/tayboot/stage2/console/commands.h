// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/console/commands.h
 *
 * Real mode console commands are declared here
 */



#pragma once

#include "../header.h"

#define CMD_NAME(x) __console_cmd_##x##__
#define DEF_CONSOLE_CMD(x) PUBLIC int CMD_NAME(x)(const char* cmdname)

DEF_CONSOLE_CMD(echo);
DEF_CONSOLE_CMD(echoln);
DEF_CONSOLE_CMD(shutdown);
DEF_CONSOLE_CMD(help);
DEF_CONSOLE_CMD(time);
DEF_CONSOLE_CMD(random);