/**
 * @file fat32.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief FAT32
 * @version alpha-1.0.0
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <fs/fs/fat32.h>
#include <string.h>
#include <logging.h>
#include <heap.h>

#define BPB_FILESYSTEM (0x52)

/**
 * @brief fat32文件系统操作表
 * 
 */
PRIVATE struct fat32op_t {
    void (*close_fs)(fs_t *fs);
    dnode_result (*get_root)(fs_t *fs);
    dnode_result (*get_next_dnode)(fs_t *fs, dnode *dir);
    dnode_result (*get_child_dnode)(fs_t *fs, dnode *dir);
    void (*get_file_data)(fs_t *fs, fnode *block, void *buffer);
} fat32op = {
    .close_fs = close_fat32_fs
};

typedef dword fat32block;

/**
 * @brief fat32文件节点
 * 
 */
typedef struct {
    fat32block block;
    void *buffer;
    size_t size;
    wchar *name;
} fat32fnode;

/**
 * @brief fat32目录节点
 * 
 */
typedef struct {
    fat32fnode *dir_file;
    fat32fnode *parent_file;
    dword next_offset;
    bool is_directory;
} fat32dnode;

/**
 * @brief FAT表缓存大小
 * 
 */
#define FAT_BUFFER_SZ (4096)
/**
 * @brief 缓存项数
 * 
 */
#define ENTRIES_PER_BUFFER (FAT_BUFFER_SZ / sizeof(dword))
/**
 * @brief 每扇区项数
 * 
 */
#define ENTRIES_PER_SECTOR (512 / sizeof(dword))
/**
 * @brief 缓存扇区数
 * 
 */
#define SECTORS_PER_BUFFER (512 / sizeof(dword))

/**
 * @brief fat32
 * 
 */
typedef struct {
    struct fat32op_t *op_tab;
    disk_t *disk;
    fat32dnode *root;
    dword data_start;
    byte spc;
    dword fat_start;
    void *fatbuffer;
    fat32block buffer_start;
} fat32fs_t;

struct _fat32pbr {
    word _jmp_short; //跳转指令 +0x00
    byte _nop; //空指令 +0x02
    char oem_name[8]; //oem名 +0x03
    word bytes_per_sector; //每扇区字节数 +0x0B
    byte sectors_per_clus; //每簇扇区数 +0x0D
    word reserved_sectors; //保留扇区 +0x0E
    byte num_fats; //fat数 +0x10
    dword reserved0; // +0x11
    byte media; //介质 +0x15
    word reserved1; // +0x16
    word sectors_per_track; //每磁道扇区数 +0x18
    word heads; //磁头数 +0x1A
    dword hidden_sectors; //隐藏扇区数 +0x1C
    dword total_sectors; //总扇区数 +0x20
    dword fat_sectors; //fat扇区数 +0x24
    word reserved2; // +0x28
    word version; //版本号 +0x2A
    dword root_directory_start_clus; //根目录区起始簇号 +0x2C
    word fsinfo_sectors; //FS信息扇区数 +0x30
    word backup_boot_sectors; //引导备份扇区数 +0x32
    dword reserved3; // +0x34
    dword reserved4; // +0x38
    dword reserved5; // +0x3C
    byte drive_no; //驱动器号 +0x40
    byte reserved6; //驱动器号 +0x41
    byte boot_sig; //boot标志 +0x42
    dword volumn_id; //卷号 +0x43
    nchar volumn_label[11]; //卷标 +0x47
    nchar file_system[8]; //文件系统 +0x52
} __attribute__((packed));

/**
 * @brief FAT32 PBR
 * 
 */
typedef struct _fat32pbr fat32pbr;

/**
 * @brief FAT32 日期
 * 
 */
struct _fat32_date {
    byte year : 7;
    byte month : 4;
    byte day : 5;
};

/**
 * @brief FAT32 时间
 * 
 */
struct _fat32_time {
    byte hour: 5;
    byte minute : 6;
    byte second : 5;
};

/**
 * @brief FAT32 文件项
 * 
 */
struct _fat32_fentry {
    char name[8];
    char ext[3];
    struct {
        bool readonly : 1;
        bool hidden : 1;
        bool system : 1;
        bool volumn_id : 1;
        bool directory : 1;
        bool archive : 1;
        byte reserved : 2;
    } attr;
    byte reserved;
    byte creation_ten_seconds;
    struct _fat32_time creation_time;
    struct _fat32_date creation_date;
    struct _fat32_date last_accessed_date;
    word start_clus_high;
    struct _fat32_time last_modification_time;
    struct _fat32_date last_modification_date;
    word start_clus_low;
    dword filesize;
} __attribute__((packed));

/**
 * @brief FAT32 LFN
 * 
 */
struct _fat32_lfn {
    byte order;       // + 0x0
    unsigned short first5[5];  // + 0x1
    byte lfn_attr;    // + 0xB
    byte lfn_type;    // + 0xC
    byte chksum;      // + 0xD
    unsigned short next6[6];   // + 0xE
    word reserved;    // + 0x1A
    unsigned short final2[2];  // + 0x1C
} __attribute__((packed));

/**
 * @brief FAT32目录项
 * 
 */
struct _fat32_entry {
    union {
        struct _fat32_fentry file;
        struct _fat32_lfn lfn;
    };
} __attribute__((packed));

/**
 * @brief FAT32目录项
 * 
 */
typedef struct _fat32_entry fat32_entry;

#define FAT32_ENTRY_SIZE (0x20)

PUBLIC bool is_fat32_fs(void *pbr) {
    char fs_type[9];
    //获取BPB中FS号
    memcpy(fs_type, pbr + BPB_FILESYSTEM, 8);
    fs_type[8] = '\0';

    return strcmp(fs_type, "FAT32   ") == 0;
}
/**
 * @brief 获得fat32项
 * 
 * @param fs 文件系统
 * @param block fat32块
 * @return fat32项
 */
fat32block __get_fat32_entry__(fat32fs_t *fs, fat32block block) {
    dword buffer_end = fs->buffer_start + ENTRIES_PER_BUFFER;
    // 缓存需要更新
    if ((fs->buffer_start == -1) || (block < fs->buffer_start) || (block > buffer_end)) {
        fs->buffer_start = (block / ENTRIES_PER_SECTOR);
        int entry_sector = fs->fat_start + fs->buffer_start;
        read_sector (fs->disk, entry_sector, SECTORS_PER_BUFFER, fs->fatbuffer);
    }
    dword offset = (block - fs->buffer_start) * sizeof(dword);
    return *(dword *)(fs->fatbuffer + offset);
}

/**
 * @brief 获得块数据
 * 
 * @param fs 文件系统
 * @param block fat32块
 * @param buffer 缓存
 */
INLINE void __get_block_data__(fat32fs_t *fs, fat32block block, void *buffer) {
    dword sector = fs->data_start;
    sector += block * fs->spc;
    read_sector(fs->disk, sector, fs->spc, buffer);
}

/**
 * @brief 获得文件数据
 * 
 * @param fs 文件系统
 * @param start_block 起始块
 * @param buffer 缓存
 */
INLINE void __get_file_data__(fat32fs_t *fs, fat32block start_block, void *buffer) {
    fat32block it = start_block;
    while (it < 0x0FFFFFF0) {
        __get_block_data__(fs, it, buffer);
        buffer += 512 * fs->spc;
        it = __get_fat32_entry__(fs, it);
    }
}

INLINE fat32fnode *__open_fat32_file__(fat32block startblock, size_t size){
    fat32fnode *file = malloc(sizeof(fat32fnode));
    file->block = startblock;
    file->size = size;
    file->buffer = NULL;
    return file;
}

/**
 * @brief 打开FAT32目录
 * 
 * @param block fat32块
 * @return FAT32目录
 */
INLINE fat32dnode *__open_fat32_dir__(fat32fnode *parent_file, size_t size, fat32block startblock, dword next_offset, wchar *name) {
    fat32dnode *dir = malloc(sizeof(fat32dnode));

    // 目录文件
    dir->dir_file = __open_fat32_file__(startblock, size);
    dir->dir_file->block = startblock;
    dir->dir_file->name = name;
    dir->parent_file = parent_file;
    dir->next_offset = next_offset;
    return dir;
}

/**
 * @brief 检测文件缓存
 * 
 * @param fs 文件系统
 * @param file 文件
 */
INLINE void __check_file_buffer__(fat32fs_t *fs, fat32fnode *file) {
    if (file->buffer == NULL) {
        dword size = (file->size + 511) / 512;
        file->buffer = malloc(size);
        __get_file_data__(fs, file->block, file->buffer);
    }
}

/**
 * @brief LFN属性
 * 
 */
#define LFN_ATTR (0xF)

/**
 * @brief LFN处理结果
 * 
 */
typedef struct {
    wchar *name;
    //下一项在buffer中的偏移
    dword entry_offset;
} lfn_result;

/**
 * @brief 处理LFN
 * 
 * @param fs 文件系统
 * @param buffer 缓存
 * @param offset LFN起始项偏移
 * @return LFN处理结果 
 */
INLINE lfn_result __deal_lfn__(fat32fs_t *fs, void *buffer, dword offset) {
    int lfn_num = 0;
    dword _offset = offset;
    //计算LFN项数量
    while (true) {
        fat32_entry *entry = (fat32_entry *)(buffer + _offset);
        if (entry->lfn.lfn_attr != LFN_ATTR) {
            break;
        }
        _offset += FAT32_ENTRY_SIZE;
        lfn_num ++;
    }
    //无LFN项
    if (lfn_num == 0) {
        wchar *name = malloc(sizeof(wchar));
        *name = 0x0000;
        return (lfn_result){L"", _offset};
    }
    if (lfn_num == 1) {
        unsigned short name_buf[13];
        fat32_entry *entry = (fat32_entry *)(buffer + offset);
        memcpy(name_buf, entry->lfn.first5, 10);
        memcpy(&name_buf[5], entry->lfn.next6, 12);
        memcpy(&name_buf[11], entry->lfn.final2, 4);
        int namesz = 13;
        for (int i = 0 ; i < 13 ; i ++) {
            if (name_buf[i] == 0xFFFF) {
                namesz = i;
                break;
            }
        }
        wchar *name = malloc((namesz + 1) * sizeof(wchar));
        for (int i = 0 ; i < namesz ; i ++) {
            name[i] = (name_buf[i] & 0xFFFF);
        }
        name[namesz] = L'\0';
        return (lfn_result){name, _offset};
    }
    //TODO: lfn_num >= 2
    wchar *name = malloc(sizeof(wchar));
    *name = 0x0000;
    return (lfn_result){L"", _offset};
}

/**
 * @brief 打开FAT32目录
 * 
 * @param fs 文件系统
 * @return FAT32目录
 */
INLINE fat32dnode *__open_fat32_dir_dentry__(fat32fs_t *fs, fat32fnode *parent_file, fat32block offset) {
    //TODO:
    __check_file_buffer__(fs, parent_file);
    fat32_entry *entry = (fat32_entry *)(parent_file->buffer + offset);
    
    return NULL;
}

/**
 * @brief 打开fat32文件系统
 * 
 * @param disk 磁盘
 * @param pbr 分区引导记录
 * @return fat32文件系统
 */
INLINE fat32fs_t *__open_fat32_fs__(disk_t *disk, fat32pbr *pbr) {
    fat32fs_t *fs = malloc(sizeof(fat32fs_t));
    // 操作表
    fs->op_tab = &fat32op;
    // 磁盘
    fs->disk = disk;
    // 每簇扇区数
    fs->spc = pbr->sectors_per_clus;
    // 计算各个部分的开始扇区
    fs->fat_start = pbr->reserved_sectors;
    fs->data_start = fs->fat_start + pbr->num_fats * pbr->fat_sectors - 2;
    fat32block rootdir = pbr->root_directory_start_clus;

    // 缓存
    fs->fatbuffer = malloc (FAT_BUFFER_SZ);
    fs->buffer_start = -1;

    size_t root_size = 0;
    fat32block it = rootdir;
    while (it < 0x0FFFFFF0) {
        it = __get_fat32_entry__(fs, it);
        root_size += 512 * fs->spc;
    }

    // 根目录
    wchar *root_name = malloc(4);
    memcpy(root_name, L"/", 4);
    fs->root = __open_fat32_dir__(NULL, root_size, rootdir, 0, root_name);
    
    // 获得根目录缓存
    __check_file_buffer__(fs, fs->root->dir_file);
    return fs;
}

/**
 * @brief 关闭fat32文件系统
 * 
 * @param fs fat32文件系统
 */
INLINE void __close_fat32_fs__(fat32fs_t *fs) {
    free(fs->root);
    free(fs);
}

/**
 * @brief 包装dnode result
 * 
 * @param dir 目录
 * @param is_last 是否为最后一个目录
 * @param is_root 是否为根目录
 * @param is_leaf 是否为叶子节点
 * @return dnode result
 */
INLINE dnode_result __pack_dres__(fat32dnode *dir, bool is_last, bool is_root, bool is_leaf) {

    dnode_result result;
    // 设置root信息
    result.dir = (dnode *)dir;
    result.file = (fnode *)dir->dir_file;
    result.filesz = dir->dir_file->size;
    // 设置根目录名
    dword namesz = (wstrlen(dir->dir_file->name) + 1) * sizeof(wchar);
    result.name = malloc(namesz);
    memcpy(result.name, dir->dir_file->name, namesz);
    // 设置信息
    result.is_last = is_last;
    result.is_root = is_root;
    result.is_leaf = is_leaf;
    return result;
}

/**
 * @brief 获得根目录
 * 
 * @param fs 文件系统
 * @return 根目录
 */
INLINE dnode_result __get_fat32_root__(fat32fs_t *fs) {
    return __pack_dres__(fs->root, false, false, false);
}

PUBLIC fs_t *open_fat32_fs(disk_t *disk, void *pbr) {
    return (fs_t *)(__open_fat32_fs__(disk, (fat32pbr *)pbr));
}

PUBLIC void close_fat32_fs(fs_t *fs) {
    __close_fat32_fs__((fat32fs_t *)fs);
}

PUBLIC dnode_result get_fat32_root(fs_t *fs) {
    return __get_fat32_root__((fat32fs_t *)fs);   
}