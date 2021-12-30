#include "disk_extensions.h"

IF_EXT_DISK_SECTOR_INFO iedsi_list[100];

IF_EXT_DISK_SECTOR_INFO create_extension_iedsi(pdriver driver) {
    return &iedsi_list[driver->id];
}