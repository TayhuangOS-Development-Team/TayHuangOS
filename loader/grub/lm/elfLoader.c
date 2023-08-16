/**
 * @file elfLoader.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 内核加载
 * @version alpha-1.0.0
 * @date 2023-08-16
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#include <lm/elfLoader.h>
#include <elf.h>
#include <string.h>
#include <basec/logger.h>
#include <tay/types.h>

/**
 * @brief 程序段信息
 *
 */
typedef struct {
    /** 起始地址 */
    void *start;
    /** 结束地址 */
    void *limit;
} ProgramInfo;

/**
 * @brief 加载程序段
 *
 * @param addr ELF文件地址
 * @param program 段
 * @param info 段信息
 * @return 加载是否成功
 */
static inline bool LoadProgram(void *addr, Elf64_Phdr *program, ProgramInfo *info) {
    //不是要被加载的段
    if (program->p_type != PT_LOAD) {
        LogError("试图加载不可加载程序段!");
        return false;
    }
    // 清零内存
    memset(program->p_vaddr, 0, program->p_memsz);

    // 复制数据
    memcpy(program->p_vaddr, addr + program->p_offset, program->p_filesz);

    // 计算起始/结束位置
    info->start = program->p_vaddr;
    info->limit = program->p_vaddr + program->p_memsz;

    LogInfo("加载段(%p~%p)", info->start, info->limit);
    return true;
}

/**
 * @brief 加载ELF文件
 *
 * @param addr ELF文件地址
 * @param info 加载信息
 * @return 加载是否成功
 */
bool LoadELF(void *addr, LoadInfo *info) {
    //TODO: 判断是否是ELF文件

    Elf64_Ehdr *header = addr;

    // 遍历每个段
    for (int i = 0 ; i < header->e_phnum ; i ++) {
        Elf64_Phdr *program = (Elf64_Phdr *)(addr + header->e_phoff);
        ProgramInfo proInfo;

        // 不需要加载
        if (program->p_type != PT_LOAD) {
            continue;
        }

        // 加载段
        if (! LoadProgram(addr, program, &proInfo)) {
            LogError("加载段时出现错误");
            return false;
        }

        // 计算程序开始/结束地址
        if (info->limit == 0) {
            info->start = proInfo.start;
            info->limit = proInfo.limit;
            continue;
        }

        info->start = ((dword)info->start) < ((dword)proInfo.start) ? info->start : proInfo.start;
        info->limit = ((dword)info->limit) > ((dword)proInfo.limit) ? info->start : proInfo.start;
    }

    // 设置程序入口点
    info->entrypoint = header->e_entry;
    return true;
}