// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/drivers/memory/memory_driver.c
 *
 * Memory driver is implemented here
 */



#include "memory_driver.h"
#include "../../intcall.h"
#include "../../heap.h"
#include "../../printf.h"

PRIVATE bool initialize_driver(pdevice device, pdriver driver, id_t id) {
    if (driver->state != DS_UNINITIALIZE || device->type != DT_MEMORY)
        return false;
    driver->dev_ty = DT_MEMORY;
    driver->state = DS_IDLE;
    driver->extensions = NULL;
    driver->id = id;
    device->driver = driver;
    driver->device = device;
    return true;
}

PRIVATE bool prode_memory(pmem_prode result) {
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    addr_t ards_addr;
    in_regs.ebx = 0;
    in_regs.edi = ards_addr = ll_alloc(sizeof(ards_t) * 2);
    in_regs.es = ll_get_heap_seg();
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
        ll_cp_from_heap(ards_addr, &(result->ards_list[cnt ++]), 20);
        in_regs.ebx = out_regs.ebx;
    } while (out_regs.ebx);

    result->prode_cnt = cnt;

    return true;
}

PRIVATE mem_prode_t prode_result;
PRIVATE struct {
    dword mem_cnt;
} MEM_INFO;

PRIVATE struct {
    sreg_t heap_segment;
    addr_t heap_bottom;
    addr_t heap_top;
    addr_t mst_start;
    dword mst_entry_num;
} HEAP;

struct __mse_t {
    byte m_p : 1;
    word m_start : 16;
    byte m_len : 3;
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

DEF_SUB_CMD(init_heap) {
    HEAP.heap_segment = 0x7000;
    HEAP.mst_start = 0;
    HEAP.mst_entry_num = 0;
    HEAP.heap_bottom = HEAP.heap_top = MST_MAX_ENTRY_NUMBER * sizeof(mse_t);
    return true;
}

DEF_SUB_CMD(prode) {
    return prode_memory(pack);
}

DEF_SUB_CMD(get_mem_size) {
    *((dword*)pack) = MEM_INFO.mem_cnt;
    return true;
}

PRIVATE byte heap_get_byte(addr_t addr) {
    sreg_t essv = rdes();
    stes(HEAP.heap_segment);
    byte res = rdes8(addr);
    stes(essv);
    return res;
}

PRIVATE void heap_set_byte(addr_t addr, byte val) {
    sreg_t essv = rdes();
    stes(HEAP.heap_segment);
    stes8(addr, val);
    stes(essv);
}

PRIVATE void heap_cp_from(addr_t src, void* dst, word num) {
    for (word i = 0 ; i < num ; i ++) {
        *((byte*)dst + i) = heap_get_byte(src + i);
    }
}

PRIVATE void heap_cp_to(void* src, addr_t dst, word num) {
    for (word i = 0 ; i < num ; i ++) {
        heap_set_byte(src + i, *((byte*)dst + i));
    }
}

PRIVATE void heap_cp_to_heap(addr_t src, addr_t dst, word num) {
    for (word i = 0 ; i < num ; i ++) {
        heap_set_byte(src + i, heap_get_byte(dst + i));
    }
}

PRIVATE void read_mse(pmse mse, word idx) {
    heap_cp_from(HEAP.mst_start + idx * sizeof(mse_t), mse, sizeof(mse_t));
}

PRIVATE void write_mse(pmse mse, word idx) {
    heap_cp_to(mse, HEAP.mst_start + idx * sizeof(mse_t), sizeof(mse_t));
}

PRIVATE int find_free_mse(pmse mse, word start) {
    for (;start < HEAP.mst_start ; start ++) {
        read_mse(mse, start);
        if (mse->m_type == MSE_TY_FREE) {
            return start;
        }
    }
    return 0xffff;
}

PRIVATE void insert_new_mse(pmse mse) {
    write_mse(mse, HEAP.mst_entry_num);
    HEAP.mst_entry_num ++;
}

PRIVATE dword leading_zeros(dword x) {
    if (x == 0) return 32;
    int n = 1;
    if (x >> 16 == 0) {n += 16; x <<= 16;}
    if (x >> 24 == 0) {n += 8; x <<= 8;}
    if (x >> 28 == 0) {n += 4; x <<= 4;}
    if (x >> 30 == 0) {n += 2; x <<= 2;}
    n -= x >> 31;
    return n;
}

DEF_SUB_CMD(reset_heap) {
    for (dword i = 0 ; i < HEAP.heap_top ; i ++) {
        heap_set_byte(i, 0);
    }
    HEAP.heap_segment = 0x7000;
    HEAP.mst_start = 0;
    HEAP.mst_entry_num = 0;
    HEAP.heap_bottom = HEAP.heap_top = MST_MAX_ENTRY_NUMBER * sizeof(mse_t);
    return true;
}

#define HEAP_UNIT (1024)

DEF_SUB_CMD(alloc) {
    PAPACK(mm, alloc) args = (PAPACK(mm, alloc))pack;
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
        if (TO2POW(HEAP.heap_top + 1, HEAP_UNIT) - HEAP.heap_top < args->length) {
            int len = TO2POW(HEAP.heap_top + 1, HEAP_UNIT) - HEAP.heap_top;
            while (len != 0) {
                mse.m_start = HEAP.heap_top;
                mse.m_len = (dword) - leading_zeros(LOWBIT(len) - 1);
                mse.m_start += LOWBIT(len);
                insert_new_mse(&mse);
                len -= LOWBIT(len);
            }
            HEAP.heap_top = TO2POW(HEAP.heap_top, 1024);
        }
        mse.m_start = HEAP.heap_top;
        mse.m_len = sizeof(dword) * 8 - leading_zeros(args->length - 1);
        HEAP.heap_top += (1 << mse.m_len);
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
    for (word i = 0 ; i < HEAP.mst_start ; i ++) {
        read_mse(mse, i);
        if (mse->m_start == start_addr) {
            return i;
        }
    }
    return 0xffff;
}

DEF_SUB_CMD(free) {
    addr_t start_addr = *(addr_t*)pack;
    mse_t mse;
    word idx = find_mse_by_start(&mse, start_addr);
    if (idx == 0xffff) {
        return false;
    }
    if (start_addr + (1 << mse.m_len) >= HEAP.heap_top) {
        HEAP.heap_top = start_addr;
        HEAP.mst_entry_num --;
    }
    else {
        mse.m_gc = 0;
        mse.m_type = MSE_TY_FREE;
    }
    return true;
}

DEF_SUB_CMD(update) {
    mse_t mse;
    for (int i = 0 ; i < HEAP.mst_start ; i ++) {
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
    for (int i = 0 ; i < HEAP.mst_entry_num ; i ++) {
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
    for (int i = 0 ; i < HEAP.mst_entry_num ; i ++) {
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
    heap_cp_from(args->src, args->dst, args->len);
    return true;
}

DEF_SUB_CMD(set_data) {
    PAPACK(mm, set_data) args = (PAPACK(mm, set_data))pack;
    heap_cp_to(args->src, args->dst, args->len);
    return true;
}

DEF_SUB_CMD(cp_data) {
    PAPACK(mm, cp_data) args = (PAPACK(mm, cp_data))pack;
    heap_cp_to_heap(args->src, args->dst, args->len);
    return true;
}

DEF_SUB_CMD(get_heap_segment) {
    *((sreg_t*)pack) = HEAP.heap_segment;
    return true;
}

PRIVATE bool process_center(pdriver driver, word cmdty, argpack_t pack) {
    if (driver->state != DS_IDLE || driver->device->type != DT_MEMORY)
        return false;
    switch (cmdty) {
        case MM_CMD_INIT_HEAP:
            return SUB_CMD(init_heap)(driver, pack);
        case MM_CMD_PRODE:
            return SUB_CMD(prode)(driver, pack);
        case MM_CMD_GET_MEM_SIZE:
            return SUB_CMD(get_mem_size)(driver, pack);
        case MM_CMD_ALLOC:
            return SUB_CMD(alloc)(driver, pack);
        case MM_CMD_FREE:
            return SUB_CMD(free)(driver, pack);
        case MM_CMD_UPDATE:
            return SUB_CMD(update)(driver, pack);
        case MM_CMD_REFRESH_GC:
            return SUB_CMD(refresh_gc)(driver, pack);
        case MM_CMD_RESET_HEAP:
            return SUB_CMD(reset_heap)(driver, pack);
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
        case MM_CMD_GET_HEAP_SEGMENT:
            return SUB_CMD(get_heap_segment)(driver, pack);
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
    while(! prode_memory(&prode_result));
    dword mem_cnt = 0;
    for (int i = 0 ; i < prode_result.prode_cnt ; i ++) {
        mem_cnt = max(mem_cnt, prode_result.ards_list[i].base_address_low + prode_result.ards_list[i].length_low);
    }
    MEM_INFO.mem_cnt = mem_cnt;
}