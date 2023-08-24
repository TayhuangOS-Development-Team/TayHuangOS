/**
 * @file elf_loader.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 内核加载
 * @version alpha-1.0.0
 * @date 2023-08-16
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#include <lm/elf_loader.h>
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
    void *programStartAddress;
    /** 结束地址 */
    void *programLimitAddress;
} ProgramInfo;

/**
 * @brief 加载程序段
 *
 * @param elfAddress ELF文件地址
 * @param program 段
 * @param info 段信息
 * @return 加载是否成功
 */
static inline bool LoadProgram(void *elfAddress, Elf64_Phdr *program, ProgramInfo *info) {
    //不是要被加载的段
    if (program->p_type != PT_LOAD) {
        LogError("试图加载不可加载程序段!");
        return false;
    }
    // 清零内存
    memset(program->p_vaddr, 0, program->p_memsz);

    // 复制数据
    memcpy(program->p_vaddr, elfAddress + program->p_offset, program->p_filesz);

    // 计算起始/结束位置
    info->programStartAddress = program->p_vaddr;
    info->programLimitAddress = program->p_vaddr + program->p_memsz;

    LogInfo("加载段(%p~%p)", info->programStartAddress, info->programLimitAddress);
    return true;
}

/**
 * @brief 加载ELF文件
 *
 * @param elfAddress ELF文件地址
 * @param info 加载信息
 * @return 加载是否成功
 */
bool LoadELF(void *elfAddress, LoadInfo *info) {
    //TODO: 判断是否是ELF文件
    Elf64_Ehdr *header = (Elf64_Ehdr *)elfAddress;

    // 遍历每个段
    for (int i = 0 ; i < header->e_phnum ; i ++) {
        Elf64_Phdr *program = (Elf64_Phdr *)(elfAddress + header->e_phoff);
        ProgramInfo proInfo;

        // 不需要加载
        if (program->p_type != PT_LOAD) {
            continue;
        }

        // 加载段
        if (! LoadProgram(elfAddress, program, &proInfo)) {
            LogError("加载段时出现错误");
            return false;
        }

        // 计算程序开始/结束地址
        if (info->programLimitAddress == 0) {
            info->programStartAddress = proInfo.programStartAddress;
            info->programLimitAddress = proInfo.programLimitAddress;
            continue;
        }

        info->programStartAddress = ((dword)info->programStartAddress) < ((dword)proInfo.programStartAddress) ? info->programStartAddress : proInfo.programStartAddress;
        info->programLimitAddress = ((dword)info->programLimitAddress) > ((dword)proInfo.programLimitAddress) ? info->programLimitAddress : proInfo.programLimitAddress;
    }

    // 设置程序入口点
    info->programEntrypoint = header->e_entry;
    return true;
}