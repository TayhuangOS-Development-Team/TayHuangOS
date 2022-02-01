// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/console/console.c
 *
 * 实模式控制台
 * 没什么实际作用
 */



#include "console.h"
#include "../printf.h"
#include "../tools.h"
#include "../drivers/disk/disk_driver.h"
#include "../drivers/drivers.h"
#include "../scanf.h"
#include "../heap.h"
#include "commands.h"
#include <string.h>
#include <ctype.h>

#ifndef REALMODE_CONSOLE_MAJOR_VERSION
#define REALMODE_CONSOLE_MAJOR_VERSION 1
#define REALMODE_CONSOLE_MINOR_VERSION 1
#define REALMODE_CONSOLE_REVISION_VERSION 1
#define REALMODE_CONSOLE_BUILD_VERSION 1
#endif

PRIVATE char splash[33];

PRIVATE void load_splash(void) { //加载标语
    int cnt = 0;
    FILE *file = fopen("SPLASHESTXT", NULL);
    while (feof(file) == 0) {
        fgetline(file, splash);
        cnt ++;
    }
    fseek(file, 0, SEEK_SET);
    struct time_t time;
    get_time(&time);
    int choice = random(time.second + time.minute * 60 + time.hour * 24 + get_clock_time(), 0, cnt);
    int t = 0;
    while (feof(file) == 0) {
        fgetline(file, splash);
        t ++;
        if (t == choice) break;
    }
    fclose (file);
}

#define SPLASH_PRINT_LINE 5

PRIVATE void print_splash(void) { //打印标语
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

PUBLIC char user_name[32] = "guest";
PUBLIC bool logined = false;

PRIVATE void print_intro(void) { //打印简介
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

PRIVATE void print_time(void) { //打印启动时间
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

PRIVATE void chkerr(int err_code) { //检查是否正常退出
    if (err_code == 0) return;
    byte old_color = get_print_color();
    set_print_color(0x0C);
    if (err_code == 1 || err_code == -1) {
        printf ("Abnormal program exit!(Exit code %d)\n", err_code); //不正常退出
    }
    else if (err_code == 2) {
        printf ("Program exit because of wrong action!\n"); //错误操作
    }
    else if (err_code == 3) {
        printf ("Wrong arguments!\n"); //错误参数
    }
    else if (err_code == -2) {
        printf ("Insufficient permissions!\n"); //权限不足
    }
    else if (err_code > 0) {
        printf ("Program exit with custom exit code (%d)\n", err_code); //自定义正常退出码
    }
    else {
        printf ("Abnormal program exit with custom exit code (%d)\n", err_code); //自定义非正常退出码
    }
    set_print_color(old_color);
}

PRIVATE void get_argn(char *arg, int size) { //获取一个参数
    char ch = getchar();
    int i;
    if (ch == '"') { //以"开头
        for (i = 0 ; i < size ; i ++) {
            arg[i] = getchar();
            if (arg[i] == '"') { //以"结束
                break;
            }
            if (arg[i] == '\n' || arg[i] == '\r') { //换行
                putchar('>'); //继续键入
            }
        }
    }
    else { //正常开头
        arg[0] = ch;
        for (i = 1 ; i < size ; i ++) {
            arg[i] = getchar();
            if (isspace(arg[i])) { //空格结束
                backchar(arg[i]);
                break;
            }
        }
    }
    arg[i] = '\0';
}


PRIVATE int get_commandsn(char** args, int size) { //获取一行参数
    int i = 0;
    while ((size --) > 0) {
        args[i] = malloc(96);
        get_argn(args[i ++], 96);
        char ch = getchar();
        if (ch == '\n' || ch == '\r') { //直至换行
            break;
        }
    }
    return i;
}

PRIVATE void deal_cmd(void) {
    char** _args = calloc(10, sizeof(char*));
    int num = get_commandsn(_args, 10); //获取参数
    if (num == 0) { //空操作
        free (_args);
        return;
    }
    const char** args = (const char**)_args;
    //判断命令
    if (! (strcmp(args[0], "echo"))) {
        chkerr(CMD_NAME(echo)(num, args));
    }
    else if (! (strcmp(args[0], "echoln"))) {
        chkerr(CMD_NAME(echoln)(num, args));
    }
    else if (! (strcmp(args[0], "shutdown"))) {
        chkerr(CMD_NAME(shutdown)(num, args));
    }
    else if (! (strcmp(args[0], "help"))) {
        chkerr(CMD_NAME(help)(num, args));
    }
    else if (! (strcmp(args[0], "time"))) {
        chkerr(CMD_NAME(time)(num, args));
    }
    else if (! (strcmp(args[0], "random"))) {
        chkerr(CMD_NAME(random)(num, args));
    }
    else if (! (strcmp(args[0], "change_name"))) {
        chkerr(CMD_NAME(change_name)(num, args));
    }
    else if (! (strcmp(args[0], "reboot"))) {
        chkerr(CMD_NAME(reboot)(num, args));
    }
    else if (! (strcmp(args[0], "guess_number"))) {
        chkerr(CMD_NAME(guess_number)(num, args));
    }
    else if (! (strcmp(args[0], "cls"))) {
        chkerr(CMD_NAME(cls)(num, args));
    }
    else if (! (strcmp(args[0], "set_pwd"))) {
        chkerr(CMD_NAME(set_pwd)(num, args));
    }
    else if (! (strcmp(args[0], "login"))) {
        chkerr(CMD_NAME(login)(num, args));
    }
    else if (! (strcmp(args[0], "ls"))) {
        chkerr(CMD_NAME(ls)(num, args));
    }
    else if (! (strcmp(args[0], "set"))) {
        chkerr(CMD_NAME(set)(num, args));
    }
    else if (! (strcmp(args[0], "goto_os"))) {
        chkerr(CMD_NAME(goto_os)(num, args));
    }
    else {
        //未知命令
        printf ("unknown command \"%s\"!\n", args[0]);
        //解析结果
        printf ("Parse result:\n");
        printf ("    Command: %s\n", args[0]);
        printf ("    Args:\n");
        for (int i = 1 ; i < num ; i ++) {
            printf ("        [%d]:%s\n", i - 1, args[i]);
        }
    }
    free (_args);
    clear_buffer(); //清除缓冲区
}

PUBLIC void enter_console(void) {
    print_intro(); //打印简介
    load_splash(); //加载标语
    print_splash(); //打印标语
    print_time(); //打印启动时间
    change_pos(0, 7); //更改位置
    logined = false; //默认未登陆
    while (true) {
        byte old_color = get_print_color();

        if (! logined) { //没登陆用绿色
            set_print_color(0x0A);
        }
        else { //登陆用红色
            set_print_color(0x0C);
        }

        printf ("realmode%c%s>", logined ? '@' : ':', user_name); //没登陆用@
                                                                  //登陆用:

        set_print_color(old_color);

        deal_cmd(); //处理命令
    }
}