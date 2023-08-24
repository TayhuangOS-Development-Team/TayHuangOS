/**
 * @file paging.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 分页
 * @version alpha-1.0.0
 * @date 2023-08-21
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

#include <stdbool.h>
#include <tay/types.h>

/** 4K页面地址掩码 */
#define PAGE_ENTRY_4K_MASK   (0x01FFFFFFFFFFF000)

/** 2M页面地址掩码 */
#define PAGE_ENTRY_2M_MASK   (0x01FFFFFFFFE00000)

/** 1G页面地址掩码 */
#define PAGE_ENTRY_1G_MASK (0x01FFFFFFC0000000)

/** 页表地址掩码 */
#define PAGING_TABLE_ENTRY_MASK (0x01FFFFFFC0000000)

/**
 * @brief 4K页面项
 *
 */
typedef union {
    struct {
        /**
         * @brief Present(存在位) bit 0
         * P=1 该页存在
         *
         */
        bool P   : 1;
        /**
         * @brief Read&Write (读写位) bit 1
         * RW=0 不允许写
         * RW=1 可读可写
         *
         */
        bool RW  : 1;
        /**
         * @brief User&Supervisor (用户/管理) bit 2
         * US=0 Supervisor页面
         * US=1 User页面
         *
         */
        bool US  : 1;
        /**
         * @brief Page-level Writes Transparent(页级别写透明) bit 3
         * PWT=1时 缓存使用直写方式写页
         * PWT=0时 缓存使用写回方式写页
         *
         */
        bool PWT : 1;
        /**
         * @brief Page-level Cache Disable(页级别缓存禁用) bit 4
         * PCD=1时 禁止使用缓存
         *
         */
        bool PCD : 1;
        /**
         * @brief Acessed (已访问) bit 5
         * A=1时 说明程序访问过该页面
         *
         */
        bool A   : 1;
        /**
         * @brief Dirty (脏) bit 6
         * D=1时 说明该页已被写入过
         *
         */
        bool D   : 1;
        /**
         * @brief Page Attribute Table(页属性表) bit 7
         * 对应页的页属性为IA32_PAT[i]
         * i=(PAT << 2) + (PCD << 1) + PWT
         *
         */
        bool PAT : 1;
        /**
         * @brief Global(全局) bit 8
         * G=1时 该页为全局页
         *
         */
        bool G   : 1;
        /** 保留 bit 9~10*/
        word reserved0 : 2;
        /**
         * @brief Restart(重启) bit 11
         * 仅对HLAT页有效
         *
         */
        bool R : 1;
        /**
         * @brief 地址 bit 12~56
         *
         */
        qword _address : 45;
        /** 保留 bit 57~58*/
        word reserved1 : 2;
        /**
         * @brief Protection Key(保护密钥) bit 59~62
         *
         */
        byte PK : 4;
        /**
         * @brief Execute-Disable(禁止执行) bit 63
         * XD=1时 禁止执行该页面
         *
         */
        bool XD : 1;
    };
    /**
     * @brief 4K页面页面的地址
     * 使用时需要 address & PAGE_ENTRY_4K_MASK
     *
     */
    qword address;
} PageEntry4K;

/**
 * @brief 2M页面项
 *
 */
typedef union {
    struct {
        /**
         * @brief Present(存在位) bit 0
         * P=1 该页存在
         *
         */
        bool P   : 1;
        /**
         * @brief Read&Write (读写位) bit 1
         * RW=0 不允许写
         * RW=1 可读可写
         *
         */
        bool RW  : 1;
        /**
         * @brief User&Supervisor (用户/管理) bit 2
         * US=0 Supervisor页面
         * US=1 User页面
         *
         */
        bool US  : 1;
        /**
         * @brief Page-level Writes Transparent(页级别写透明) bit 3
         * PWT=1时 缓存使用直写方式写页
         * PWT=0时 缓存使用写回方式写页
         *
         */
        bool PWT : 1;
        /**
         * @brief Page-level Cache Disable(页级别缓存禁用) bit 4
         * PCD=1时 禁止使用缓存
         *
         */
        bool PCD : 1;
        /**
         * @brief Acessed (已访问) bit 5
         * A=1时 说明程序访问过该页面
         *
         */
        bool A   : 1;
        /**
         * @brief Dirty (脏) bit 6
         * D=1时 说明该页已被写入过
         *
         */
        bool D   : 1;
        /**
         * @brief Page Size(页大小) bit 7
         * PS=0 => 指向PT
         * PS=1 => 指向2MB页面
         *
         */
        bool PS : 1;
        /**
         * @brief Global(全局) bit 8
         * G=1时 该页为全局页
         *
         */
        bool G   : 1;
        /** 保留 bit 9~10*/
        word reserved0 : 2;
        /**
         * @brief Restart(重启) bit 11
         * 仅对HLAT页有效
         *
         */
        bool R : 1;
        /**
         * @brief Page Attribute Table(页属性表) bit 12
         * 对应页的页属性为IA32_PAT[i]
         * i=(PAT << 2) + (PCD << 1) + PWT
         *
         */
        bool PAT : 1;
        /** 保留 bit 13~20*/
        dword reserved1 : 8;
        /**
         * @brief 地址 bit 21~56
         *
         */
        qword _address : 36;
        /** 保留 bit 57~58*/
        word reserved2 : 2;
        /**
         * @brief Protection Key(保护密钥) bit 59~62
         *
         */
        byte PK : 4;
        /**
         * @brief Execute-Disable(禁止执行) bit 63
         * XD=1时 禁止执行该页面
         *
         */
        bool XD : 1;
    };
    /**
     * @brief 2MB页面页面的地址
     * 使用时需要 address & PAGE_ENTRY_2M_MASK
     *
     */
    qword address;
} PageEntry2M;

/**
 * @brief 1G页面项
 *
 */
typedef union {
    struct {
        /**
         * @brief Present(存在位) bit 0
         * P=1 该页存在
         *
         */
        bool P   : 1;
        /**
         * @brief Read&Write (读写位) bit 1
         * RW=0 不允许写
         * RW=1 可读可写
         *
         */
        bool RW  : 1;
        /**
         * @brief User&Supervisor (用户/管理) bit 2
         * US=0 Supervisor页面
         * US=1 User页面
         *
         */
        bool US  : 1;
        /**
         * @brief Page-level Writes Transparent(页级别写透明) bit 3
         * PWT=1时 缓存使用直写方式写页
         * PWT=0时 缓存使用写回方式写页
         *
         */
        bool PWT : 1;
        /**
         * @brief Page-level Cache Disable(页级别缓存禁用) bit 4
         * PCD=1时 禁止使用缓存
         *
         */
        bool PCD : 1;
        /**
         * @brief Acessed (已访问) bit 5
         * A=1时 说明程序访问过该页面
         *
         */
        bool A   : 1;
        /**
         * @brief Dirty (脏) bit 6
         * D=1时 说明该页已被写入过
         *
         */
        bool D   : 1;
        /**
         * @brief Page Size(页大小) bit 7
         * PS=0 => 指向PT
         * PS=1 => 指向2MB页面
         *
         */
        bool PS : 1;
        /**
         * @brief Global(全局) bit 8
         * G=1时 该页为全局页
         *
         */
        bool G   : 1;
        /** 保留 bit 9~10*/
        word reserved0 : 2;
        /**
         * @brief Restart(重启) bit 11
         * 仅对HLAT页有效
         *
         */
        bool R : 1;
        /**
         * @brief Page Attribute Table(页属性表) bit 12
         * 对应页的页属性为IA32_PAT[i]
         * i=(PAT << 2) + (PCD << 1) + PWT
         *
         */
        bool PAT : 1;
        /** 保留 bit 13~29*/
        dword reserved1 : 17;
        /**
         * @brief 地址 bit 30~56
         *
         */
        qword _address : 27;
        /** 保留 bit 57~58*/
        word reserved2 : 2;
        /**
         * @brief Protection Key(保护密钥) bit 59~62
         *
         */
        byte PK : 4;
        /**
         * @brief Execute-Disable(禁止执行) bit 63
         * XD=1时 禁止执行该页面
         *
         */
        bool XD : 1;
    };
    /**
     * @brief 1GB页面页面的地址
     * 使用时需要 address & PAGE_ENTRY_1G_MASK
     *
     */
    qword address;
} PageEntry1G;

/**
 * @brief 页表项
 *
 */
typedef union {
    struct {
        /**
         * @brief Present(存在位) bit 0
         * P=1 该页存在
         *
         */
        bool P   : 1;
        /**
         * @brief Read&Write (读写位) bit 1
         * RW=0 不允许写
         * RW=1 可读可写
         *
         */
        bool RW  : 1;
        /**
         * @brief User&Supervisor (用户/管理) bit 2
         * US=0 Supervisor页面
         * US=1 User页面
         *
         */
        bool US  : 1;
        /**
         * @brief Page-level Writes Transparent(页级别写透明) bit 3
         * PWT=1时 缓存使用直写方式写页
         * PWT=0时 缓存使用写回方式写页
         *
         */
        bool PWT : 1;
        /**
         * @brief Page-level Cache Disable(页级别缓存禁用) bit 4
         * PCD=1时 禁止使用缓存
         *
         */
        bool PCD : 1;
        /**
         * @brief Acessed (已访问) bit 5
         * A=1时 说明程序访问过该页面
         *
         */
        bool A   : 1;
        /** 保留 bit 6 */
        bool reserved1   : 1;
        /**
         * @brief Page Size(页大小) bit 7
         * PS=0 => 指向PT
         * PS=1 => 指向2MB页面
         *
         */
        bool PS : 1;
        /** 保留 bit 8~10*/
        word reserved2 : 3;
        /**
         * @brief Restart(重启) bit 11
         * 仅对HLAT页有效
         *
         */
        bool R : 1;
        /**
         * @brief 地址 bit 12~56
         *
         */
        qword _address : 45;
        /** 保留 bit 57~62*/
        word reserved3 : 6;
        /**
         * @brief Execute-Disable(禁止执行) bit 63
         * XD=1时 禁止执行该页面
         *
         */
        bool XD : 1;
    };
    /**
     * @brief 页表的地址
     * 使用时需要 address & PAGING_TABLE_ENTRY_MASK
     *
     */
    qword address;
} PagingTableEntry;

/**
 * @brief 获得4K页面地址
 *
 * @param page4k 4K页面
 * @return 4K页面地址
 */
static inline dword Get4KPageAddress(PageEntry4K page4k) {
    return page4k.address & PAGE_ENTRY_4K_MASK;
}

/**
 * @brief 获得2M页面地址
 *
 * @param page2m 2M页面
 * @return 2M页面地址
 */
static inline dword Get2MPageAddress(PageEntry2M page2m) {
    return page2m.address & PAGE_ENTRY_2M_MASK;
}

/**
 * @brief 获得1G页面地址
 *
 * @param page1g 1G页面
 * @return 1G页面地址
 */
static inline dword Get1GPageAddress(PageEntry1G page1g) {
    return page1g.address & PAGE_ENTRY_1G_MASK;
}

/**
 * @brief 获得页表地址
 *
 * @param pagingTable 页表
 * @return 页表地址
 */
static inline dword GetPagingTableAddress(PagingTableEntry pagingTable) {
    return pagingTable.address & PAGING_TABLE_ENTRY_MASK;
}

/**
 * @brief PTE
 * 
 */
typedef union {
    /**
     * @brief 指向4K页
     * 
     */
    PageEntry4K refPageEntry;
} PTE;

/**
 * @brief PDE
 * 
 */
typedef union {
    /**
     * @brief 指向2M页
     * 
     */
    PageEntry2M refPageEntry;
    /**
     * @brief 指向PT
     * 
     */
    PagingTableEntry refPTEntry;
} PDE;

/**
 * @brief PDPTE
 * 
 */
typedef union {
    /**
     * @brief 指向1G页
     * 
     */
    PageEntry1G refPageEntry;
    /**
     * @brief 指向PD
     * 
     */
    PagingTableEntry refPDEntry;
} PDPTE;

/**
 * @brief PML4E
 * 
 */
typedef union {
    /**
     * @brief 指向PDPT
     * 
     */
    PagingTableEntry refPDPTEntry;
} PML4E;

/**
 * @brief PML5E
 * 
 */
typedef union {
    /**
     * @brief 指向PML4E
     * 
     */
    PagingTableEntry refPML4EEntry;
} PML5E;

/**
 * @brief 页表大小
 * 
 */
#define PAGE_TABLE_SIZE    (4096)
/**
 * @brief 页大小
 * 
 */
#define PAGE_SIZE    (4096)
/**
 * @brief 每表PTE数
 * 
 */
#define PTEPerTable    (PAGE_TABLE_SIZE / sizeof(PTE))
/**
 * @brief 每表PDE数
 * 
 */
#define PDEPerTable    (PAGE_TABLE_SIZE / sizeof(PDE))
/**
 * @brief 每表PDPTE数
 * 
 */
#define PDPTEPerTable  (PAGE_TABLE_SIZE / sizeof(PDPTE))
/**
 * @brief 每表PML4E数
 * 
 */
#define PML4EPerTable  (PAGE_TABLE_SIZE / sizeof(PML4E))
/**
 * @brief 每表PML5E数
 * 
 */
#define PML5EPerTable  (PAGE_TABLE_SIZE / sizeof(PML5E))

/**
 * @brief PT
 * 
 */
typedef PTE PT[PTEPerTable];
/**
 * @brief PD
 * 
 */
typedef PDE PD[PDEPerTable];
/**
 * @brief PDPT
 * 
 */
typedef PDPTE PDPT[PDPTEPerTable];
/**
 * @brief PML4
 * 
 */
typedef PML4E PML4[PML4EPerTable];
/**
 * @brief PML5
 * 
 */
typedef PML5E PML5[PML5EPerTable];