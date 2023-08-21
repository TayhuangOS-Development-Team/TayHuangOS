/**
 * @file main.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 内核主函数
 * @version alpha-1.0.0
 * @date 2023-08-13
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#include <stdbool.h>
#include <tay/boot.h>
#include <basec/logger.h>

void Init(void) {
}

void Terminate(void) {
}

int main(void) {
    return 0;
}

/**
 * @brief 内核启动函数
 *
 */
void setup(void) {
    register int magic __asm__("eax"); //GRUB Loader 魔数 存放在eax

    // 设置栈
    asm volatile ("movl $0x1008000, %esp");

    if ((magic & 0xFFFFFFFF) != BOOT_MAGIC) { //魔数不匹配
        while (true);
    }

    // 初始化
    Init();

    // 主函数
    int ret = main();
    if (ret != 0) {
        LogFatal("加载器发生错误!");
    }

    // 收尾
    Terminate();
}