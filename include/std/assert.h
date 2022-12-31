/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong 383494
 * 
 * assert.h
 * 
 * assert与panic
 * 
 */



#pragma once

void assertion_failure(const char *expression, const char *file, const char *base_file, int line);
void panic_failure(const char *expression, const char *file, const char *base_file, int line);
void panic(const char *format, ...);

#ifdef NDEBUG
    #define assert(expression) ((void)0)
    #define panic_assert(expression) ((void)0)
#else
    #define assert(expression) if (! (expression)) \
        assertion_failure(#expression, __FILE__, __BASE_FILE__, __LINE__)
    #define panic_assert(expression) if (! (expression)) \
        panic_failure(#expression, __FILE__, __BASE_FILE__, __LINE__)
#endif
