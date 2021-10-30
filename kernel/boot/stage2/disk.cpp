#include "disk.h"
#include "memory.h"

struct FsInfo {
    char oem_name[8];
    word bytes_per_sector;
    byte sectors_per_clus;
    word rsvd_sectors;
    byte fats_num;
    word root_entries;
    word sectors_small;
    byte media;
    word fat_sectors;
    word sectors_per_track;
    word heads;
    dword hidden_sectors;
    dword sectors_large;
    byte drive;
    byte bootsig;
    dword volumn_id;
    char volumn_label[11];
    char filesystem[8];
} fs_info;

void _readsector(dword LDA_LOW, dword LDA_HIGH, word segment, word offset);
void readsector(dword LDA_LOW, dword LDA_HIGH, word segment, word offset) {
    _readsector(LDA_LOW, LDA_HIGH, segment, offset);
    ed_callasm();
}

void initFsInfo(void) {
    readsector(0, 0, BASE_OF_DATA, 0);
    getbytearr((byte*)fs_info.oem_name, 8, BASE_OF_DATA, 0x03);
    fs_info.bytes_per_sector = getword(BASE_OF_DATA, 0x0B);
    fs_info.sectors_per_clus = getbyte(BASE_OF_DATA, 0x0D);
    fs_info.rsvd_sectors = getword(BASE_OF_DATA, 0x0E);
    fs_info.fats_num = getbyte(BASE_OF_DATA, 0x10);
    fs_info.root_entries = getword(BASE_OF_DATA, 0x11);
    fs_info.sectors_small = getword(BASE_OF_DATA, 0x13);
    fs_info.media = getbyte(BASE_OF_DATA, 0x15);
    fs_info.fat_sectors = getword(BASE_OF_DATA, 0x16);
    fs_info.sectors_per_track = getword(BASE_OF_DATA, 0x18);
    fs_info.heads = getword(BASE_OF_DATA, 0x1A);
    fs_info.hidden_sectors = getdword(BASE_OF_DATA, 0x1C);
    fs_info.sectors_large = getdword(BASE_OF_DATA, 0x20);
    fs_info.drive = getbyte(BASE_OF_DATA, 0x24);
    fs_info.bootsig = getbyte(BASE_OF_DATA, 0x26);
    fs_info.volumn_id = getdword(BASE_OF_DATA, 0x27);
    getbytearr((byte*)fs_info.volumn_label, 11, BASE_OF_DATA, 0x2B);
    getbytearr((byte*)fs_info.filesystem, 8, BASE_OF_DATA, 0x36);
}