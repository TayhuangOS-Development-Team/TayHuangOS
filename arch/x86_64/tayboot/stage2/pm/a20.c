// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/pm/a20.c
 *
 * A20函数
 */



#include "a20.h"
#include "../intcall.h"
#include <tayboot/ports.h>

#define A20_TEST_ADDRESS (0x80 << 2) //测试地址
#define A20_FAST_TEST_TIME (2 << 4) //快速测试次数
#define A20_TEST_TIME (2 << 8) //正常测试次数
#define A20_SLOW_TEST_TIME (2 << 14) //慢测试次数
#define A20_LONG_TEST_TIME (2 << 20) //久测试次数

PUBLIC bool test_a20(int loop_time) {//引用自linux
    stfs(0x0000);
    stgs(0xffff);

    int save, control;
    save = control = rdfs32(A20_TEST_ADDRESS);

    bool flag = false;

    while (loop_time --) {
        stfs32(A20_TEST_ADDRESS, ++ control);
        io_delay();
        flag = (rdgs32(A20_TEST_ADDRESS + 0x10) ^ control) != 0; //检测是否有回滚
        if (flag)
            break;
    }

    stfs32(A20_TEST_ADDRESS, save);
    return flag;
}

PUBLIC bool test_a20_fast(void) {
    return test_a20(A20_FAST_TEST_TIME);
}

PUBLIC bool test_a20_normal(void) {
    return test_a20(A20_TEST_TIME);
}

PUBLIC bool test_a20_slow(void) { //慢测试
    return test_a20(A20_SLOW_TEST_TIME);
}

PUBLIC bool test_a20_long(void) { 
    return test_a20(A20_LONG_TEST_TIME);
}

#undef A20_TEST_ADDRESS
#undef A20_FAST_TEST_TIME
#undef A20_TEST_TIME
#undef A20_SLOW_TEST_TIME
#undef A20_LONG_TEST_TIME

#define MAX_TRY_TIME (2 << 20)
#define MAX_FF_TIME (2 << 8)

PUBLIC bool empty_8042(void) {//引用自linux
    int loop_time = MAX_TRY_TIME;
    int ff_time = MAX_FF_TIME;

    while (loop_time --) {
        io_delay();
        b8 status = inb(KEYBOARD_8042_STATUS);
        if (status == 0xFF) { //错误
            if (! (-- ff_time)) {
                return false;
            }
        }
        if (status & 1) { //还有
            io_delay();
            inb(KEYBOARD_8042_DATA0);
        }
        else if (!(status & 2)) //没了
            return true;
    }
    return false;
}

#undef MAX_TRY_TIME
#undef MAX_FF_TIME

PUBLIC void enable_a20_by_bios(void) {//参考自linux
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    in_regs.eax = 0x2401;
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    args.int_no = 0x15;
    intcall(&args); //BIOS中断
}

PUBLIC void enable_a20_by_8042(void) {//引用自linux
    empty_8042(); //清空8042

    outb(KEYBOARD_8042_STATUS, 0xD1); //写STATUS
    empty_8042(); //清空8042

    outb(KEYBOARD_8042_DATA0, 0xDF); //写DATA
    empty_8042(); //清空8042

    outb(CO_CPU_F, 0x64); //写协处理器
    empty_8042(); //清空8042
}

PUBLIC void enable_a20_by_portA(void) {//引用自linux
    b8 port_a;

    port_a = inb(PORT_A);
    port_a = (port_a | 0x02) & ~0x01; //portA

    outb(PORT_A, port_a);
}

#define MAX_TRY_TIME (0xFF)

PUBLIC bool enable_a20(void) {//参考自linux
    int loop_time = MAX_TRY_TIME;

    while (loop_time --) {
        if (test_a20_fast()) //是否已经打开
            return true;

        enable_a20_by_bios(); //先用BIOS

        io_delay();
        io_delay();
        io_delay();
        io_delay();
        io_delay(); //延迟

        if (test_a20_fast()) //测试
            return true;

        if (empty_8042()) { //清空8042
            enable_a20_by_8042(); //靠8042打开
            if (test_a20_long()) //测试
                return true;
        }

        enable_a20_by_portA(); //靠portA打开
        if (test_a20_long()) //测试
            return true;
    }
    return false;
}

#undef MAX_TRY_TIME