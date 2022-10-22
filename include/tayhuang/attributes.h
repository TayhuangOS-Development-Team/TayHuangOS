/**
 * @file attributes.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 属性
 * @version alpha-1.0.0
 * @date 2022-10-22
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#define PUBLIC                                        //公共
#define API(source) extern                            //API函数
#define IMPL(source)                                  //实现API
#define PRIVATE static                                //私有
#define EXTERN extern                                 //extern
#define VOLATILE volatile                             //volatile
#define NORETURN                                      //不返回
#define DISCARDED                                     //废弃的
#define NONNULL                                       //不为空
#define NULLABLE(meaning)                             //可空的
#define UNREACHABLE                                   //不可到达的
#define AUTOMATIC                                     //原子的
#define INLINE static inline                          //内连的
#define ASSEMBLY(file, line)                          //用汇编实现的
#define RETVAL(value, meaning)                        //返回值
#define ARCHITECT(architect)                          //基于某个架构的
#define IMPL_BY_USERS                                 //由使用者实现