// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/console/console.c
 *
 * Real mode console here
 */



#include "console.h"
#include "../printf.h"
#include "../tools.h"
#include "../drivers/disk/disk_driver.h"
#include "../drivers/drivers.h"
#include "../scanf.h"
#include "commands.h"
#include <string.h>

#ifndef REALMODE_CONSOLE_MAJOR_VERSION
#define REALMODE_CONSOLE_MAJOR_VERSION 1
#define REALMODE_CONSOLE_MINOR_VERSION 1
#define REALMODE_CONSOLE_REVISION_VERSION 1
#define REALMODE_CONSOLE_BUILD_VERSION 1
#endif

PRIVATE char splash[33];

PRIVATE void load_splash(void) {
    APACK(dk, load_file) pack;
    pack.name = "SPLASHESTXT";
    pack.offset = 0;
    pack.segment = 0x1000;
    a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_LOAD_FILE, &pack);
    int cnt = 0;
    for (int i = 0 ; i < 512 ; i ++) {
        stfs(0x1000);
        if (rdfs8(i) == '\n') {
            cnt ++;
        }
    }
    struct time_t time;
    get_time(&time);
    int choice = random(time.second + time.minute * 60 + time.hour * 24 + get_clock_time(), 0, cnt);
    int t = 0;
    for (int i = 0 ; i < 512 ; i ++) {
        stfs(0x1000);
        if (t == choice) {
            int j = 0;
            while (rdfs8(i) != '\n') {
                splash[j ++] = rdfs8(i ++);
            }
            splash[j] = 0;
            break;
        }
        if (rdfs8(i) == '\n') {
            t ++;
        }
    }
}

#define SPLASH_PRINT_LINE 5

PRIVATE void print_splash(void) {
    word old_x = get_pos_x();
    word old_y = get_pos_y();
    byte old_color = get_print_color();

    change_pos(0, SPLASH_PRINT_LINE);
    set_print_color(0x0C);

    printf ("%s", splash);

    change_pos(old_x, old_y);
    set_print_color(old_color);
}

#define INTRO_PRINT_LINE 0

PRIVATE void print_intro(void) {
    word old_x = get_pos_x();
    word old_y = get_pos_y();
    byte old_color = get_print_color();

    change_pos(0, INTRO_PRINT_LINE);
    set_print_color(0x09);

    printf ("TayhuangOS Real Mode Console  [Version %d.%d.%d Build %d]\n",
        REALMODE_CONSOLE_MAJOR_VERSION, REALMODE_CONSOLE_MINOR_VERSION, REALMODE_CONSOLE_REVISION_VERSION, REALMODE_CONSOLE_BUILD_VERSION);
    printf ("                SPDX-License-Identifier: GPL-2.0-only                     \n");
    printf ("-------------------------------*-TayhuangOS-*-----------------------------\n");
    printf ("Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved\n");
    printf ("--------------------------------------------------------------------------\n");

    set_print_color(old_color);
    change_pos(old_x, old_y);
}

#define TIME_PRINT_LINE 6

PRIVATE void print_time(void) {
    word old_x = get_pos_x();
    word old_y = get_pos_y();
    byte old_color = get_print_color();

    change_pos(0, TIME_PRINT_LINE);
    set_print_color(0x09);

    printf("Boot time:");

    set_print_color(0x0D);

    struct time_t time;
    struct date_t date;
    get_time(&time);
    get_date(&date);

    printf("%d/%d/%d %d:%d:%d", date.year, date.month, date.day, time.hour, time.minute, time.second);

    set_print_color(old_color);
    change_pos(old_x, old_y);
}

PRIVATE void chkerr(int err_code) {
    if (err_code == 0) return;
    if (err_code == 1 || err_code == -1) {
        printf ("Abnormal program exit!(Exit code %d)\n", err_code);
    }
    else if (err_code == 2) {
        printf ("Program exit because of wrong action!");
    }
    else if (err_code > 0) {
        printf ("Program exit with custom exit code (%d)", err_code);
    }
    else {
        printf ("Abnormal program exit with custom exit code (%d)", err_code);
    }
}

PRIVATE void deal_cmd(void) {
    char cmd[32];
    scanf ("%s", cmd);
    if (! (strcmp(cmd, "echo"))) {
        chkerr(CMD_NAME(echo)(cmd));
    }
    else if (! (strcmp(cmd, "echoln"))) {
        chkerr(CMD_NAME(echoln)(cmd));
    }
    else if (! (strcmp(cmd, "shutdown"))) {
        chkerr(CMD_NAME(shutdown)(cmd));
    }
    else if (! (strcmp(cmd, "help"))) {
        chkerr(CMD_NAME(help)(cmd));
    }
    else {
        printf ("unknown command \"%s\"!\n", cmd);
    }
    clear_buffer();
}

PUBLIC void enter_console(void) {
    print_intro();
    load_splash();
    print_splash();
    print_time();
    change_pos(0, 7);
    while (true) {
        byte old_color = get_print_color();

        set_print_color(0x0A);

        printf ("realmode:%s@%s>", "anonymous", "/");

        set_print_color(old_color);

        deal_cmd();
    }
}