/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * memory_driver.c
 *
 * 内存驱动
 *
 */



#include "memory_driver.h"
#include "../../intcall.h"
#include "../../buffer.h"
#include "../../printf.h"

PRIVATE mem_prode_t prode_result;
PRIVATE struct {
    dword mem_cnt;
    dword mem_cnt_high;
} MEM_INFO;

PUBLIC dword get_memsz_high(void) {
    return MEM_INFO.mem_cnt_high;
}

PRIVATE bool prode_memory(pmem_prode result) {
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    addr_t ards_addr;
    in_regs.ebx = 0;
    in_regs.edi = ards_addr = ll_alloc_buffer(sizeof(ards_t) * 2);
    in_regs.es = ll_get_buffer_seg();
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    int cnt = 0;
    do {
        in_regs.eax = 0xE820;
        in_regs.ecx = 20;
        in_regs.edx = 0x534D4150;
        args.int_no = 0x15;
        intcall(&args);
        if (EF_GETCF(out_regs.eflags) != 0) {
            return false;
        }
        ll_cp_from_buffer(ards_addr, &(result->ards_list[cnt ++]), 20);
        in_regs.ebx = out_regs.ebx;
    } while (out_regs.ebx);

    result->prode_cnt = cnt;

    return true;
}

PRIVATE bool initialize_driver(pdevice device, pdriver driver, id_t id) {
    if (driver->state != DS_UNINITIALIZE || device->type != DT_MEMORY)
        return false;
    driver->dev_ty = DT_MEMORY;
    driver->state = DS_IDLE;
    driver->extensions = NULL;
    driver->id = id;
    device->driver = driver;
    driver->device = device;
    while(! prode_memory(&prode_result));
    dword mem_cnt = 0;
    dword mem_cnt_high = 0;
    for (int i = 0 ; i < prode_result.prode_cnt ; i ++) {
        mem_cnt = max(mem_cnt, prode_result.ards_list[i].base_address_low + prode_result.ards_list[i].length_low);
        mem_cnt_high = max(mem_cnt, prode_result.ards_list[i].base_address_high + prode_result.ards_list[i].length_high);
    }
    MEM_INFO.mem_cnt = mem_cnt;
    MEM_INFO.mem_cnt_high = mem_cnt_high;
    return true;
}

PRIVATE struct {
    sreg_t buffer_segment;
    addr_t buffer_bottom;
    addr_t buffer_top;
    addr_t mst_start;
    dword mst_entry_num;
} BUFFER;

struct __mse_t {
    word m_start : 16;
    byte m_len : 4;
    byte m_gc : 2;
    byte m_type : 2;
} __attribute__((packed));

typedef struct __mse_t mse_t;
typedef struct __mse_t *pmse;

#define MSE_TY_FREE 0
#define MSE_TY_USING 1
#define MSE_TY_WEAK 2
#define MSE_TY_ERROR 3

#define MST_MAX_ENTRY_NUMBER (2048)

DEF_SUB_CMD(init_buffer) {
    BUFFER.buffer_segment = 0x7000;
    BUFFER.mst_start = 0;
    BUFFER.mst_entry_num = 0;
    BUFFER.buffer_bottom = BUFFER.buffer_top = MST_MAX_ENTRY_NUMBER * sizeof(mse_t);
    return true;
}

DEF_SUB_CMD(prode) {
    return prode_memory(pack);
}

DEF_SUB_CMD(get_mem_size) {
    *((dword*)pack) = MEM_INFO.mem_cnt;
    return true;
}

PRIVATE byte buffer_get_byte(addr_t addr) {
    sreg_t essv = rdes();
    stes(BUFFER.buffer_segment);
    byte res = rdes8(addr);
    stes(essv);
    return res;
}

PRIVATE void buffer_set_byte(addr_t addr, byte val) {
    sreg_t essv = rdes();
    stes(BUFFER.buffer_segment);
    stes8(addr, val);
    stes(essv);
}

PRIVATE void buffer_cp_from(addr_t src, void *dst, word num) {
    for (word i = 0 ; i < num ; i ++) {
        *((byte*)dst + i) = buffer_get_byte(src + i);
    }
}

PRIVATE void buffer_cp_to(void *src, addr_t dst, word num) {
    for (word i = 0 ; i < num ; i ++) {
        buffer_set_byte(dst + i, *((byte*)src + i));
    }
}

PRIVATE void buffer_cp_to_buffer(addr_t src, addr_t dst, word num) {
    for (word i = 0 ; i < num ; i ++) {
        buffer_set_byte(dst + i, buffer_get_byte(src + i));
    }
}

PRIVATE void read_mse(pmse mse, word idx) {
    buffer_cp_from(BUFFER.mst_start + idx * sizeof(mse_t), mse, sizeof(mse_t));
}

PRIVATE void write_mse(pmse mse, word idx) {
    buffer_cp_to(mse, BUFFER.mst_start + idx * sizeof(mse_t), sizeof(mse_t));
}

PRIVATE int find_free_mse(pmse mse, word start) {
    for (;start < BUFFER.mst_start ; start ++) {
        read_mse(mse, start);
        if (mse->m_type == MSE_TY_FREE) {
            return start;
        }
    }
    return 0xffff;
}

PRIVATE void insert_new_mse(pmse mse) {
    write_mse(mse, BUFFER.mst_entry_num);
    BUFFER.mst_entry_num ++;
}

DEF_SUB_CMD(reset_buffer) {
    for (dword i = 0 ; i < BUFFER.buffer_top ; i ++) {
        buffer_set_byte(i, 0);
    }
    BUFFER.buffer_segment = 0x7000;
    BUFFER.mst_start = 0;
    BUFFER.mst_entry_num = 0;
    BUFFER.buffer_bottom = BUFFER.buffer_top = MST_MAX_ENTRY_NUMBER * sizeof(mse_t);
    return true;
}

DEF_SUB_CMD(alloc_buffer) {
    PAPACK(mm, alloc_buffer) args = (PAPACK(mm, alloc_buffer))pack;
    if (args->length == 0) {
        args->address = 0;
        return false;
    }
    mse_t mse;
    int idx = 0;
    do {
        idx = find_free_mse(&mse, idx);
        if (idx == 0xffff) {
            break;
        }
    }while (args->length > (1 << mse.m_len));

    if (idx == 0xffff) {
        mse.m_start = BUFFER.buffer_top;
        mse.m_len = sizeof(dword) * 8 - leading_zeros(args->length - 1);
        BUFFER.buffer_top += ((word)1 << (word)mse.m_len);
        insert_new_mse(&mse);
    }

    if (args->weak) {
        mse.m_type = MSE_TY_WEAK;
        mse.m_gc = 3;
    }
    else {
        mse.m_type = MSE_TY_USING;
        mse.m_gc = 0;
    }
    write_mse(&mse, idx);

    *(args->address) = mse.m_start;

    return true;
}

PRIVATE word find_mse_by_start(pmse mse, addr_t start_addr) {
    for (word i = 0 ; i < BUFFER.mst_start ; i ++) {
        read_mse(mse, i);
        if (mse->m_start == start_addr) {
            return i;
        }
    }
    return 0xffff;
}

DEF_SUB_CMD(free_buffer) {
    addr_t start_addr = *(addr_t*)pack;
    mse_t mse;
    word idx = find_mse_by_start(&mse, start_addr);
    if (idx == 0xffff) {
        return false;
    }
    if (start_addr + (1 << mse.m_len) >= BUFFER.buffer_top) {
        BUFFER.buffer_top = start_addr;
        BUFFER.mst_entry_num --;
    }
    else {
        mse.m_gc = 0;
        mse.m_type = MSE_TY_FREE;
    }
    return true;
}

DEF_SUB_CMD(update) {
    mse_t mse;
    for (int i = 0 ; i < BUFFER.mst_start ; i ++) {
        read_mse(&mse, i);
        if (mse.m_type == MSE_TY_WEAK) {
            if (mse.m_gc == 0) {
                mse.m_type = MSE_TY_FREE;
            }
            else {
                mse.m_gc --;
            }
            write_mse(&mse, i);
        }
    }
    return true;
}

DEF_SUB_CMD(refresh_gc) {
    mse_t mse;
    word idx = find_mse_by_start(&mse, *(addr_t*)pack);
    if (idx == 0xffff) {
        return false;
    }
    if (mse.m_type == MSE_TY_WEAK) {
        mse.m_gc = 3;
        write_mse(&mse, idx);
    }
    return true;
}

DEF_SUB_CMD(get_used) {
    dword used_cnt = 0;
    mse_t mse;
    for (int i = 0 ; i < BUFFER.mst_entry_num ; i ++) {
        read_mse(&mse, i);
        if (mse.m_type != MSE_TY_FREE) {
            used_cnt += (1 << mse.m_len);
        }
    }
    return true;
}

DEF_SUB_CMD(get_remain) {
    dword remain_cnt = 0;
    mse_t mse;
    for (int i = 0 ; i < BUFFER.mst_entry_num ; i ++) {
        read_mse(&mse, i);
        if (mse.m_type == MSE_TY_FREE) {
            remain_cnt += (1 << mse.m_len);
        }
    }
    *(dword*)pack = remain_cnt;
    return true;
}

DEF_SUB_CMD(get_data) {
    PAPACK(mm, get_data) args = (PAPACK(mm, get_data))pack;
    buffer_cp_from(args->src, args->dst, args->size);
    return true;
}

DEF_SUB_CMD(set_data) {
    PAPACK(mm, set_data) args = (PAPACK(mm, set_data))pack;
    buffer_cp_to(args->src, args->dst, args->size);
    return true;
}

DEF_SUB_CMD(cp_data) {
    PAPACK(mm, cp_data) args = (PAPACK(mm, cp_data))pack;
    buffer_cp_to_buffer(args->src, args->dst, args->size);
    return true;
}

DEF_SUB_CMD(get_buffer_segment) {
    *((sreg_t*)pack) = BUFFER.buffer_segment;
    return true;
}

PRIVATE bool process_center(pdriver driver, word cmdty, argpack_t pack) {
    if (driver->state != DS_IDLE || driver->device->type != DT_MEMORY)
        return false;
    switch (cmdty) {
        case MM_CMD_INIT_BUFFER:
            return SUB_CMD(init_buffer)(driver, pack);
        case MM_CMD_PRODE:
            return SUB_CMD(prode)(driver, pack);
        case MM_CMD_GET_MEM_SIZE:
            return SUB_CMD(get_mem_size)(driver, pack);
        case MM_CMD_ALLOC:
            return SUB_CMD(alloc_buffer)(driver, pack);
        case MM_CMD_FREE:
            return SUB_CMD(free_buffer)(driver, pack);
        case MM_CMD_UPDATE:
            return SUB_CMD(update)(driver, pack);
        case MM_CMD_REFRESH_GC:
            return SUB_CMD(refresh_gc)(driver, pack);
        case MM_CMD_RESET_BUFFER:
            return SUB_CMD(reset_buffer)(driver, pack);
        case MM_CMD_GET_USED:
            return SUB_CMD(get_used)(driver, pack);
        case MM_CMD_GET_REMAIN:
            return SUB_CMD(get_remain)(driver, pack);
        case MM_CMD_GET_DATA:
            return SUB_CMD(get_data)(driver, pack);
        case MM_CMD_SET_DATA:
            return SUB_CMD(set_data)(driver, pack);
        case MM_CMD_CP_DATA:
            return SUB_CMD(cp_data)(driver, pack);
        case MM_CMD_GET_BUFFER_SEGMENT:
            return SUB_CMD(get_buffer_segment)(driver, pack);
    }
    return false;
}

PRIVATE bool terminate_driver(pdriver driver) {
    if (driver->state == DS_TERMAINATED || driver->device->type != DT_MEMORY)
        return false;
    while (true);
    driver->state = DS_TERMAINATED;
    return true;
}

PUBLIC void create_memory_driver(pdriver driver) {
    driver->init_handle = (initialize_handle_t)initialize_driver;
    driver->pc_handle = (pc_handle_t)process_center;
    driver->terminate_handle = (terminate_handle_t)terminate_driver;
    driver->state = DS_UNINITIALIZE;
}