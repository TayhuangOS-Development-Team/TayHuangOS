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
 * key_parser.c
 * 
 * 解析器
 * 
 */



#include <key_parser.h>
#include <keymap.h>

#include <debug/logging.h>

PUBLIC int state;

PUBLIC short do_normal(byte code) {
    key_t key;
    if (code == 0xE0) { //e0
        linfo ("flags += e0_xx");
        state = E0_XX;
        return 0;
    }
    else if (code == 0xE1) { //e1
        linfo ("flags += e1_xx");
        state |= E1_XX;
        return 0;
    }
    else if (code & 0x80) { //break code
        return 0;
    }

    code &= 0x7F;
    key = KEYMAP[code][0];
    
    if (key == LSHIFT) { //lshift pushed
        linfo ("flags += lshift_pushed");
        state |= LSHIFT_PUSHED;
        return 0;
    }
    else if (key == RSHIFT) { //rshift pushed
        linfo ("flags += rshift_pushed");
        state |= RSHIFT_PUSHED;
        return 0;
    }

    if ((key >= 'a' && key <= 'z')) {
        if (state & CAPSLOCKING) { //a~z + Capslock => 大写
            key = KEYMAP[code][1];
        }
    }

    if (key >= PAD_HOME && key <= PAD_DOT) {
        if (state & NUMLOCKING) { //PAD_HOME~PAD_DOT + Numlock => 数字
            key = KEYMAP[code][1];
        }
    }

    return key;
}

PUBLIC short do_e0(byte code) {
    if (code & 0x80) { //break code
        return 0;
    }

    code &= 0x7F;

    key_t key = KEYMAP[code][2];

    linfo ("flags -= e0_xx");
    state &= ~E0_XX;

    return key;
}

//Pause Break Code序列
PRIVATE byte pausebreak_codes[] = {
    0xE1, 0x1D, 0x45,
    0xE1, 0x9D, 0xC5
};

PUBLIC short do_e1(byte code) {
    linfo ("flags += e1_stage_unit"); //增加stage
    state += E1_STAGE_UNIT;

    int stage = (state & E1_STAGE_MASK) / E1_STAGE_UNIT; //当前stage
    if (code != pausebreak_codes[stage]) { //code是否符合
        linfo ("flags += e1_failed"); //不符合则失败
        state |= E1_FAILED;
    }

    if ((state & E1_STAGE_MASK) == E1_STAGE_END) { //读完
        key_t key = PAUSEBREAK;
        
        linfo ("flags -= e1_stages"); //重置state
        state &= ~E1_STAGE_MASK;
        linfo ("flags -= e1_xx");
        state &= ~E1_XX;

        if ((state & E1_FAILED) != 0) { //失败
            linfo ("flags -= e1_failed");
            state &= ~E1_FAILED;
            return 0;
        }
            
        return key;
    }

    return 0;
}

PUBLIC short do_shift(byte code) {
    if (code == 0xE0) { //e0
        linfo ("flags += e0_xx");
        state |= E0_XX;
        return 0;
    }
    else if (code == 0xE1) { //e1
        linfo ("flags += e1_xx");
        state |= E1_XX;
        return 0;
    }
    else if (code & 0x80) { //break code
        code &= 0x7F;
        key_t key = KEYMAP[code][0];

        if (key == LSHIFT) { //lshift release
            linfo ("flags -= lshift_pushed");
            state &= ~LSHIFT_PUSHED;
            return 0;
        }
        
        if (key == RSHIFT) { //rshift release
            linfo ("flags -= rshift_pushed");
            state &= ~RSHIFT_PUSHED;
            return 0;
        }
        return 0;
    }
    
    code &= 0x7F;
    key_t key = KEYMAP[code][1];

    if (key == LSHIFT) { //lshift pushed
        linfo ("flags += lshift_pushed");
        state |= LSHIFT_PUSHED;
        return 0;
    } 
    else if (key == RSHIFT) { //rshift pushed
        linfo ("flags += rshift_pushed");
        state |= RSHIFT_PUSHED;
        return 0;
    }

    return key;
}