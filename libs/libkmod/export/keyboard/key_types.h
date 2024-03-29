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
 * key_types.h
 * 
 * 按键类型
 * 
 */



#pragma once

typedef short key_t;

enum {
    ESC = 256,
    BACKSPACE,
    TAB,
    ENTER,
    PAD_ENTER,
    LCTRL,
    RCTRL,
    LSHIFT,
    RSHIFT,
    LALT,
    RALT,
    PAD_SLASH,
    CAPSLOCK,
    NUMLOCK,
    SCROLL_LOCK,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    PAD_HOME,
    HOME,
    PAD_UP,
    KEY_UP,
    PAD_PGUP,
    PGUP,
    PAD_MINUS,
    PAD_LEFT,
    KEY_LEFT,
    PAD_MID,
    PAD_RIGHT,
    KEY_RIGHT,
    PAD_PLUS,
    PAD_END,
    KEY_END,
    PAD_DOWN,
    KEY_DOWN,
    PAD_PGDOWN,
    PGDOWN,
    PAD_INSERT,
    KEY_INSERT,
    PAD_DOT,
    KEY_DELETE,
    F11,
    F12,
    LGUI,
    RGUI,
    APPS,
    PAUSEBREAK
};