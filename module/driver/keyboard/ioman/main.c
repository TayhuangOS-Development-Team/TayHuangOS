/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong
 * 
 * main.c
 * 
 * 主函数
 * 
 */



#include <debug/logging.h>

#include <syscall/syscall.h>
#include <syscall/ipc.h>
#include <syscall/rpc.h>

#include <memory/malloc.h>

#include <tayhuang/io.h>
#include <tayhuang/ports.h>
#include <tayhuang/services.h>

#include <fifo.h>
#include <keymap.h>
#include <key_parser.h>
#include <export/__ioman.h>

//缓冲区
PRIVATE void *fifo = NULL;

PRIVATE void irq_handler(int irq) {
    linfo ("fuck!");

    if (irq == 1) {
        byte code = inb(KEYBOARD_8042_DATA0); //读取操作码
        short key;
        
        if (fifo == NULL) {
            return;
        }
        
        //E1开头的scan code序列
        if ((state & E1_XX) != 0) {
            key = do_e1(code);
        }
        else if ((state & E0_XX) != 0) { //E0开头的scan code序列
            key = do_e0(code);
        }
        else if (((state & LSHIFT_PUSHED) != 0) || ((state & RSHIFT_PUSHED) != 0)) { //按下shift
            key = do_shift(code);
        }
        else {
            key = do_normal(code);
        }

        if (key == CAPSLOCK) { //大写锁定
            if (state & CAPSLOCKING) { //release
                linfo ("flags -= capslocking");
                state &= ~CAPSLOCKING;
            }
            else { //set
                linfo ("flags += capslocking");
                state |= CAPSLOCKING;
            }
            return;
        }
        else if (key == NUMLOCK) { //小键盘锁定
            if (state & NUMLOCKING) { //release
                linfo ("flags -= numlocking");
                state &= ~NUMLOCKING;
            }
            else { //set
                linfo ("flags += numlocking");
                state |= NUMLOCKING;
            }
            return;
        }
        else if (key == 0) {
            return;
        }

        //写到fifo中
        fifo_write_bytes(fifo, (byte*)&key, sizeof(short));
    }
}

#define KEY_BUFFER_SIZE (8192)

PRIVATE void *__share_keybuffer(int caller, bool flag) {
    if (caller != KEYBOARD_DRIVER_SERVICE) { //只与键盘驱动分享Key Buffer
        return NULL;
    }

    return share_fifo(fifo, caller);
}

PUBLIC rpc_args_struct wrapper_share_keybuffer(int service, rpc_func func, rpc_args_struct args) {
    bool flag = ARG_READ(args.data, bool);

    void **buffer = malloc(sizeof(void *));
    *buffer = __share_keybuffer(service, flag);

    return (rpc_args_struct){.data = buffer, .size = sizeof(SHARE_KEYBUFFER_RETURN_TYPE)};
}

PRIVATE void init_keyboard(void) {
    // 重置Output buffer
    outb (KEYBOARD_8042_DATA0, 0xF4);

    // 启用键盘
    outb (KEYBOARD_8042_CONTROL, 0xAE);

    // 读取status
    outb(KEYBOARD_8042_CONTROL, 0x20);

    //ACK
    if (inb(0x60) != 0xFA) { 
        linfo ("Error!");
        return;
    }

    //获取status
    byte status = inb(0x60); 
    status |= 1;

    //设置status
    outb(KEYBOARD_8042_CONTROL, 0x60); 
    outb(KEYBOARD_8042_DATA0, status);
}

PUBLIC void kmod_main(void) {
    set_logging_name("Keyboard IO Manager");
    
    //注册IRQ=1
    reg_irq(1);
    register_irq_handler(irq_handler);

    //设置RPC函数
    rpc_register(SHARE_KEYBUFFER_FN, wrapper_share_keybuffer, sizeof(SHARE_KEYBUFFER_RETURN_TYPE), SHARE_KEYBUFFER_ARGS_SIZE);

    //创建缓冲区
    //FIXME: 新的RPC机制使得必须要先进入message loop才可以使用rpc函数
    fifo = create_fifo(KEY_BUFFER_SIZE);

    //设置状态
    state = NORMAL;

    //初始化硬盘
    init_keyboard();

    message_loop();

    while (true);
}