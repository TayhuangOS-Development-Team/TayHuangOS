// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/console/commands.c
 *
 * Real mode console commands are implemented here
 */



#include "commands.h"
#include <string.h>
#include "../heap.h"
#include <ctype.h>
#include "../tools.h"
#include "../printf.h"
#include "../scanf.h"
#include "../intcall.h"
#include "../drivers/disk/disk_driver.h"
#include "../drivers/drivers.h"

typedef struct {
    char* name;
    char* value;
    void* nxt;
} cmd_var;

PRIVATE cmd_var* cmd_variables;

PRIVATE void delete_variable(char *name) {
    cmd_var* current_var = cmd_variables;
    if (cmd_variables->nxt == NULL) return;
    while (strcmp(((cmd_var*)current_var->nxt)->name, name)) {
        current_var = current_var->nxt;
    }
    cmd_var* del_var = current_var->nxt;
    current_var->nxt = del_var->nxt;
    free(del_var->name);
    free(del_var->value);
    free(del_var);
}

PRIVATE bool terminate_variables(void) {
    while (cmd_variables->nxt != NULL) {
        cmd_var* nxt = cmd_variables->nxt;
        delete_variable(nxt->name);
    }
    free(cmd_variables);
    return true;
}

PUBLIC void init_variables(void) {
    cmd_variables = malloc(sizeof(cmd_var));
    register_terminater(terminate_variables);
}

PRIVATE void insert_variable(const char *name, const char *value) {
    cmd_var* current_var = cmd_variables;
    while (current_var->nxt != NULL) {
        current_var = current_var->nxt;
    }
    current_var->nxt = malloc(sizeof(cmd_var));
    cmd_var* new_var = current_var->nxt;
    new_var->name = malloc(strlen(name) + 1);
    strcpy(new_var->name, name);
    new_var->value = malloc(strlen(value) + 1);
    strcpy(new_var->value, value);
}

cmd_var* lookup_variable(const char* name) {
    cmd_var* current_var = cmd_variables->nxt;
    while (current_var != NULL) {
        if (strcmp(current_var->name, name) == 0) {
            break;
        }
        current_var = current_var->nxt;
    }
    return current_var;
}

PRIVATE void set_variable(const char *name, const char *value) {
    cmd_var *var = lookup_variable(name);
    if (var == NULL) {
        return;
    }
    free(var->value);
    var->value = malloc(strlen(value) + 1);
    strcpy(var->value, value);
}

PRIVATE const char* rd_var_namen(const char* src, char* output, int n) {
    while (isalnum(*src) && (n > 0)) {
        *output = *src;
        src ++;
        output ++;
        n --;
    }
    *output = '\0';
    return src;
}

PRIVATE void do_echo(const char* sentence) {
    while (*sentence != '\0') {
        if (*sentence == '\\') {
            sentence ++;
            if (*sentence != '\0') {
                putchar(escape(sentence));
            }
            else {
                break;
            }
            sentence ++;
        }
        else if (*sentence == '$') {
            char name[20];
            sentence = rd_var_namen(++ sentence, name, 19);
            if (lookup_variable(name) == NULL) {
                printf (" ");
            }
            else {
                printf ("%s", lookup_variable(name)->value);
            }
            if (*sentence == '$') {
                sentence ++; //实现$name$
            }
        }
        else {
            putchar (*sentence);
            sentence ++;
        }
    }
}

DEF_CONSOLE_CMD(echo) {
    if (argc != 2)
        return 3;
    do_echo(argv[1]);
    return 0;
}

DEF_CONSOLE_CMD(echoln) {
    if (argc != 2)
        return 3;
    do_echo(argv[1]);
    printf ("\n");
    return 0;
}

DEF_CONSOLE_CMD(shutdown) {
    if (! logined)
        return -2;
    if (argc != 1)
        return 3;
    intargs_t args;
    reg_collect_t in_regs, out_regs;
    in_regs.eax = MKDWORD(0, 0x5307);
    in_regs.ebx = MKDWORD(0, MKWORD(0, 1));
    in_regs.ecx = MKDWORD(0, 0x0003);
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    args.int_no = 0x15; 
    intcall(&args);
    return -1;
}

DEF_CONSOLE_CMD(time) {
    if (argc != 1)
        return 3;
    struct time_t time;
    struct date_t date;
    get_time(&time);
    get_date(&date);

    printf("Current Time: %d/%d/%d %d:%d:%d\n", date.year, date.month, date.day, time.hour, time.minute, time.second);
    return 0;
}

DEF_CONSOLE_CMD(random) {
    if (argc != 3)
        return 3;
    int min, max;
    min = atoi(argv[1]);
    max = atoi(argv[2]);
    struct time_t time;
    struct date_t date;
    get_time(&time);
    get_date(&date);
    int seed = date.year * 365 + date.month * 30 + date.day + time.second + time.minute * 60 + time.hour * 24 + get_clock_time();
    int choice = random(seed, min, max + 1);
    printf ("Random Number: %d\n", choice);
    return 0;
}

extern PUBLIC char user_name[32];

DEF_CONSOLE_CMD(change_name) {
    if (! logined)
        return -2;
    if (argc != 1)
        return 3;
    strcpy(user_name, argv[1]);
    return 0;
}

DEF_CONSOLE_CMD(help) {
    printf ("help: show help\n");
    printf ("echo [sentence]: write sentence into screen\n");
    printf ("echoln [sentence]: write sentence and \\n into screen\n");
    printf ("shutdown: power off\n");
    printf ("time: print current time\n");
    printf ("random [min] [max]: print random number\n");
    printf ("change_name [new name]: change current user name\n");
    return 0;
}

DEF_CONSOLE_CMD(reboot) {
    if (! logined)
        return -2;
    if (argc != 1)
        return 3;
    asmv ("ljmp $0xffff, $0");
    return -1;
}

DEF_CONSOLE_CMD(guess_number) {
    if (argc != 1)
        return 3;
    struct time_t time;
    struct date_t date;
    get_time(&time);
    get_date(&date);
    int seed = date.year * 365 + date.month * 30 + date.day + time.second + time.minute * 60 + time.hour * 24 + get_clock_time();
    int fun = random (seed ++, 0, 3);

    int times, min, max;
    printf ("Input the guess times:");
    scanf ("%d", &times);
    printf ("Input the range of number[min, max]:");
    scanf ("%d%d", &min, &max);
    
    int num = random (fun * (seed ++), min, max + 1);

    if (fun == 0) printf ("I have chosen a number, good luck:\n");
    else if (fun == 1) printf ("The number is already, guess it!\n");
    else if (fun == 2) printf ("You can't guess the number!\n");

    while (times --) {
        if (fun == 0) printf ("Input your number:");
        else if (fun == 1) printf ("Input the number please:");
        else if (fun == 2) printf ("Input the number, although it's impossible be the right one:");

        int guess;
        scanf ("%d", &guess);
        if (guess == num) {
            if (fun == 0) printf ("Good Job!The right number is %d!\n", num);
            else if (fun == 1) printf ("Perfect!It's %d!\n", num);
            else if (fun == 2) printf ("It's impossible!How can you know it's %d?!\n", num);
            return 0;
        }
        else if (guess < num) {
            if (fun == 0) printf ("Don't give up!It's just too small!\n");
            else if (fun == 1) printf ("Too small!Try again!\n");
            else if (fun == 2) printf ("You can't guess the right number by typing a small number!\n");
        }
        else if (guess > num) {
            if (fun == 0) printf ("Don't give up!It's just too large!\n");
            else if (fun == 1) printf ("Too large!Try again!\n");
            else if (fun == 2) printf ("You can't guess the right number by typing a large number!\n");
        }
    }
    if (fun == 0) printf ("Don't be discouraged, you can win next time!The number is %d\n", num);
    else if (fun == 1) printf ("It's a pity, you don't guess the right number!The right one is %d\n", num);
    else if (fun == 2) printf ("Look, you don't guess the right one!I win!The number is %d!\n", num);
    return 0;
}

DEF_CONSOLE_CMD(cls) {
    if (argc != 1)
        return 3;
    clrscr();
    return 0;
}

extern PUBLIC bool print_key_typed;

PUBLIC dword hash_pwd(const char* pwd) {
    dword _pwd = random(strlen(pwd), 0, 62);
    while (*pwd != '\0') {
        if (islower(*pwd)) {
            _pwd = _pwd * 62 + (*pwd) - 'a';
        }
        else if (isupper(*pwd)) {
            _pwd = _pwd * 62 + (*pwd) - 'A' + 26;
        }
        else if (isdigit(*pwd)) {
            _pwd = _pwd * 62 + (*pwd) - '0' + 52;
        }
        pwd ++;
    }
    return _pwd;
}

DEF_CONSOLE_CMD(set_pwd) {
    if (argc != 1)
        return 3;
    if (! logined)
        return -2;
    FILE *fp = fopen("REALMODEPWD", "w");
    if (fp->file_info->length != 0) {
        dword pwd;
        fscanf (fp, "%u", &pwd);
        print_key_typed = false;
        printf ("Input your old password:\n");
        char input[21];
        scanf("%s", input);
        dword _input = hash_pwd(input);
        print_key_typed = true;
        if (_input != pwd) {
            printf ("verify failed!\n");
            fclose(fp);
            return 1;
        }
        fseek(fp, 0, SEEK_SET);
    }
    printf ("Input password(Only a-z, A-Z, 0-10)(enter for reset, 1-20):\n");
    char pwd[21];
    print_key_typed = false;
    scanf("%s", pwd);
    print_key_typed = true;
    if (pwd[0] != '\0') {
        printf ("Input password again:\n");
        char pwd_again[21];
        print_key_typed = false;
        scanf("%20s", pwd_again);
        print_key_typed = true;
        if (strcmp(pwd, pwd_again)) {
            printf ("The two entered passwords do not match!\n");
            return 1;
        }
        dword _pwd = hash_pwd(pwd);
        fprintf (fp, "%u", _pwd);
    }
    fclose(fp);
    return 0;
}

DEF_CONSOLE_CMD(login) {
    if (argc != 1)
        return 3;
    if (logined){
        printf ("success!welcome back!\n");
        logined = true;
        return 0;
    }
    FILE *fp = fopen("REALMODEPWD", NULL);
    if (fp->file_info->length == 0) {
        printf ("success!welcome back!\n");
        logined = true;
        fclose (fp);
        return 0;
    }
    dword pwd;
    fscanf (fp, "%u", &pwd);
    fclose(fp);
    print_key_typed = false;
    printf ("Input your password:\n");
    char input[21];
    scanf("%s", input);
    dword _input = hash_pwd(input);
    print_key_typed = true;
    if (_input != pwd) {
        printf ("login failed!\n");
        return 1;
    }
    else {
        printf ("success!welcome back!\n");
        logined = true;
    }
    return 0;
}

DEF_CONSOLE_CMD(ls) {
    if (argc != 1)
        return 3;
    char file_name[12];
    APACK(dk, foreach_file) arg;
    arg.next = 0;
    arg.output = file_name;
    do {
        a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_FOREACH_FILE, &arg);
        if (file_name[0] != '\0')
            printf ("\"%s\"  ", file_name);
    }
    while (file_name[0] != '\0');
    printf ("\n");
    return 0;
}

DEF_CONSOLE_CMD(set) {
    if (argc != 3)
        return 3;
    if (lookup_variable(argv[1]) == NULL)
        insert_variable(argv[1], argv[2]);
    else
        set_variable(argv[1], argv[2]);
    return 0;
}