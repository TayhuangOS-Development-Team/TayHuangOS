; SPDX-License-Identifier: GPL-2.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; Author: Flysong
;
; kernel/boot/stage2/fat16head.inc
;
; Fat16 head here
;



FatSec equ 52
FatsNum equ 2
RootSectorNumber equ 0x20
EntriesPerSector equ (512 / 32)

BS_OEM_NAME: db "TayHuang" ;3-A
BPB_BPS: dw 512 ;B-C
BPB_SPC: db 1 ;D
BPB_RsvdSecCnt: dw 1 ;E-F
BPB_NumFats: db FatsNum ;10
BPB_RtEntCnt: dw RootSectorNumber * 512 / 0x20 ;11-12
BPB_TotSecS: dw 0xC4E0 ;13-14
BPB_Media: db 0xF8 ;15
BPB_FatSec: dw FatSec ;16-17
BPB_SPT: dw 32 ;18-19
BPB_HeadCnt: dw 4 ;1A-1B
BPB_HidSec: dd 0 ;1C-1F
BPB_TotSecL: dd 0 ;20-23
FS_BIOS_Drive: db 0x80 ;24
FS_Reserved1: db 0 ;25
FS_BootSig: db 0x29 ;26
FS_VolID: dd 0 ;27-2A
FS_VolLabel: db "TayhuangSys" ;2B-35
FS_FS: db "FAT16   " ;36-3E

ROOT_SECTION_START equ FatsNum * FatSec + 1
DATA_SECTION_START equ ROOT_SECTION_START + RootSectorNumber - 2