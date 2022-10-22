/**
 * @file assert.h
 * @author theflysong (song_of_the_fly@163.com) 383494
 * @brief assert与panic
 * @version alpha-1.0.0
 * @date 2022-10-22
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
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
