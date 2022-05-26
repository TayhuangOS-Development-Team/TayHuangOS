;
; SPDX-License-Identifier: GPL-3.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; 作者: Flysong
;
; fat16head.inc
;
; Fat16头
;



FatSec equ 52 ;Fat区大小
FatsNum equ 2 ;Fat区数量(第二个作为备份)
RootSectorNumber equ 0x20 ;根目录区扇区数
EntriesPerSector equ (512 / 32) ;每扇区项数

BS_OEM_NAME: db "TayHuang" ;3-A 厂商名
BPB_BPS: dw 512 ;B-C 每扇区字节数
BPB_SPC: db 1 ;D 每簇扇区数
BPB_RsvdSecCnt: dw 1 ;E-F 保留扇区数
BPB_NumFats: db FatsNum ;10 FAT数
BPB_RtEntCnt: dw RootSectorNumber * EntriesPerSector ;11-12 根目录项数
BPB_TotSecS: dw 0xC4E0 ;13-14 总扇区数
BPB_Media: db 0xF8 ;15 介质
BPB_FatSec: dw FatSec ;16-17 FAT扇区数
BPB_SPT: dw 32 ;18-19 每磁道扇区数
BPB_HeadCnt: dw 4 ;1A-1B 磁头数
BPB_HidSec: dd 0 ;1C-1F 隐藏扇区数
BPB_TotSecL: dd 0 ;20-23 总扇区数(>=65536)
FS_BIOS_Drive: db 0x80 ;24 驱动设备号
FS_Reserved1: db 0 ;25 保留
FS_BootSig: db 0x29 ;26 启动标志
FS_VolID: dd 0 ;27-2A 卷标
FS_VolLabel: db "TayhuangSys" ;2B-35 卷名
FS_FS: db "FAT16   " ;36-3E 文件系统名

ROOT_SECTION_START equ FatsNum * FatSec + 1 ; 根目录区开始
DATA_SECTION_START equ ROOT_SECTION_START + RootSectorNumber - 2 ; 数据区开始