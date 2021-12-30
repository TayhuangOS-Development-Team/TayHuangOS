#pragma once

#include "../driver.h"

typedef struct {
    addr_t address;
} IF_EXT_DISK_SECTOR_INFO;

IF_EXT_DISK_SECTOR_INFO create_extension_iedsi(pdriver driver);