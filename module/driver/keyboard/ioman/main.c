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
 * keyboard 主函数
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

PRIVATE void *fifo = NULL;

PRIVATE enum {
    NORMAL = 0,
    E0_XX = 1,
    E1_XX = 2,
    E1_STAGE_UNIT = 4,
    E1_STAGE_MASK = 28,
    E1_STAGE_END = 20,
    E1_FAILED = 32,
    NUMLOCKING = 64,
    CAPSLOCKING = 128,
    LSHIFT_PUSHED = 1024,
    RSHIFT_PUSHED = 2048,
} state;

PRIVATE byte pausebreak_codes[] = {
    0xE1, 0x1D, 0x45,
    0xE1, 0x9D, 0xC5
};

PRIVATE void irq_handler(int irq) {
    if (irq == 1) {
        byte code = inb(KEYBOARD_8042_DATA0);
        short key;
        
        if (fifo == NULL) {
            return;
        }
        
        else if ((state & E1_XX) != 0) {
            linfo ("flags += e1_stage_unit");
            state += E1_STAGE_UNIT;

            if (code != pausebreak_codes[(state & E1_STAGE_MASK) / E1_STAGE_UNIT]) {
                linfo ("flags += e1_failed");
                state |= E1_FAILED;
            }

            if ((state & E1_STAGE_MASK) == E1_STAGE_END) {
                key = PAUSEBREAK;
                
                linfo ("flags -= e1_stages");
                state &= ~E1_STAGE_MASK;
                linfo ("flags -= e1_xx");
                state &= ~E1_XX;

                if ((state & E1_FAILED) != 0) {
                    linfo ("flags -= e1_failed");
                    state &= ~E1_FAILED;
                    return;
                }
                    
                linfo ("Pause Break(%d, %#02X)", PAUSEBREAK, PAUSEBREAK);

                return;
            }
        }
        else if ((state & E0_XX) != 0) {
            if (code & 0x80) {
                return;
            }

            code &= 0x7F;

            key = KEYMAP[code][2];
            linfo ("Function(%d, %#02X;code=%d, %#02X)", key, key, code, code);

            linfo ("flags -= e0_xx");
            state &= ~E0_XX;
        }
        else if (((state & LSHIFT_PUSHED) != 0) || ((state & RSHIFT_PUSHED) != 0)) {
            if (code == 0xE0) {
                linfo ("flags += e0_xx");
                state |= E0_XX;
                return;
            }
            else if (code == 0xE1) {
                linfo ("flags += e1_xx");
                state |= E1_XX;
                return;
            }
            else if (code & 0x80) {
                code &= 0x7F;
                key = KEYMAP[code][0];

                if (key == LSHIFT) {
                    linfo ("flags -= lshift_pushed");
                    state &= ~LSHIFT_PUSHED;
                    return;
                }
                
                if (key == RSHIFT) {
                    linfo ("flags -= rshift_pushed");
                    state &= ~RSHIFT_PUSHED;
                    return;
                }
                return;
            }
            
            code &= 0x7F;
            key = KEYMAP[code][1];

            if (key == LSHIFT) {
                linfo ("flags += lshift_pushed");
                state |= LSHIFT_PUSHED;
                return;
            }
            else if (key == RSHIFT) {
                linfo ("flags += rshift_pushed");
                state |= RSHIFT_PUSHED;
                return;
            }

            linfo ("%c(%d, %#02X;code=%d, %#02X)", key, key, key, code, code);
        }
        else {
            if (code == 0xE0) {
                linfo ("flags += e0_xx");
                state = E0_XX;
                return;
            }
            else if (code == 0xE1) {
                linfo ("flags += e1_xx");
                state |= E1_XX;
                return;
            }
            else if (code & 0x80) {
                return;
            }

            code &= 0x7F;
            key = KEYMAP[code][0];
            
            if (key == LSHIFT) {
                linfo ("flags += lshift_pushed");
                state |= LSHIFT_PUSHED;
                return;
            }
            else if (key == RSHIFT) {
                linfo ("flags += rshift_pushed");
                state |= RSHIFT_PUSHED;
                return;
            }

            if ((key >= 'a' && key <= 'z')) {
                if (state & CAPSLOCKING) {
                    key = KEYMAP[code][1];
                }
            }

            if (key >= PAD_HOME && key <= PAD_DOT) {
                if (state & NUMLOCKING) {
                    key = KEYMAP[code][1];
                }
            }

            if (key >= 256) {  
                linfo ("Function(%d, %#02X;code=%d, %#02X)", key, key, code, code);
            }
            else {
                linfo ("%c(%d, %#02X;code=%d, %#02X)", key, key, key, code, code);
            }
        }

        if (key == CAPSLOCK) {
            if (state & CAPSLOCKING) {
                linfo ("flags -= capslocking");
                state &= ~CAPSLOCKING;
            }
            else {
                linfo ("flags += capslocking");
                state |= CAPSLOCKING;
            }
            return;
        }
        else if (key == NUMLOCK) {
            if (state & NUMLOCKING) {
                linfo ("flags -= numlocking");
                state &= ~NUMLOCKING;
            }
            else {
                linfo ("flags += numlocking");
                state |= NUMLOCKING;
            }
            return;
        }

        fifo_write_bytes(fifo, (byte*)&key, sizeof(short));
    }
}

#define KEY_BUFFER_SIZE (8192)

#define SHARE_KEYBUFFER_FN (0)

PUBLIC void *share_keybuffer(int caller, bool flag) {
    if (caller != KEYBOARD_DRIVER_SERVICE) { //只与键盘驱动分享Key Buffer
        return NULL;
    }

    return share_fifo(fifo, caller);
}

PUBLIC rpc_args_struct wrapper_share_keybuffer(int service, rpc_func func, rpc_args_struct args) {
    bool flag = ARG_READ(args.data, bool);

    void **buffer = malloc(sizeof(void *));
    *buffer = share_keybuffer(service, flag);

    return (rpc_args_struct){.data = buffer, .size = sizeof(void *)};
}

PRIVATE void init_keyboard(void) {
    outb (KEYBOARD_8042_DATA0, 0xFA);
    outb (KEYBOARD_8042_CONTROL, 0xAE);

    outb(KEYBOARD_8042_CONTROL, 0x20);
    if (inb(0x60) != 0xFA) {
        linfo ("Error!");
        return;
    }
    byte status = inb(0x60);
    status |= 1;
    outb(KEYBOARD_8042_CONTROL, 0x60);
    outb(KEYBOARD_8042_DATA0, status);
}

PUBLIC void kmod_main(void) {
    set_logging_name("Keyboard IO Manager");

    linfo ("Hi!");
    
    reg_irq(1);
    register_irq_handler(irq_handler);

    rpc_register(SHARE_KEYBUFFER_FN, wrapper_share_keybuffer, sizeof(void *), sizeof(bool));

    fifo = create_fifo(KEY_BUFFER_SIZE);
    state = NORMAL;

    init_keyboard();

    message_loop();

    while (true);
}