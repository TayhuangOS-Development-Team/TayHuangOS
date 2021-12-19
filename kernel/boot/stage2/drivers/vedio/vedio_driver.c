#include "vedio_driver.h"

PRIVATE bool initialize_driver(pdevice device, pdriver driver, id_t id) {
    if (driver->state != DS_UNINITIALIZE || device->type != DT_VEDIO)
        return false;
    driver->dev_ty = DT_VEDIO;
    driver->state = DS_IDLE;
    driver->extension = NULL;
    driver->id = id;
    return true;
}

PRIVATE bool process_read_byte_cmd(pdevice device, pdriver driver, argpack_t pack) {
    pvd_readbyte_ap_t args = (pvd_readbyte_ap_t)pack;
    stgs(0xB800);
    args->out->ch = rdgs8((args->posY * 80 + args->posX) * 2);
    args->out->color = rdgs8((args->posY * 80 + args->posX) * 2 + 1);
    return true;
}

void _clrscr(void);
PRIVATE bool process_clrscr_cmd(pdevice device, pdriver driver, argpack_t pack) {
    _clrscr();
    ed_callasm();
    return true;
}

PRIVATE bool process_write_byte_cmd(pdevice device, pdriver driver, argpack_t pack) {
    pvd_writebyte_ap_t args = (pvd_writebyte_ap_t)pack;
    stgs(0xB800);
    stgs8((args->posY * 80 + args->posX) * 2, args->ch);
    stgs8((args->posY * 80 + args->posX) * 2 + 1, args->color);
    return true;
}

PRIVATE bool process_center(pdevice device, pdriver driver, word cmdty, argpack_t pack) {
    if (driver->state == DS_TERMAINATED || device->type != DT_VEDIO)
        return false;
    switch (cmdty) {
    case VD_CMD_READ_BYTE:
        return process_read_byte_cmd(device, driver, pack);
    case VD_CMD_WRITE_BYTE:
        return process_write_byte_cmd(device, driver, pack);
    case VD_CMD_CLRSCR:
        return process_clrscr_cmd(device, driver, pack);
    }
    return false;
}

PRIVATE bool terminate_driver(pdevice device, pdriver driver) {
    if (driver->state == DS_TERMAINATED || device->type != DT_VEDIO)
        return false;
    driver->state = DS_TERMAINATED;
    return true;
}

PUBLIC void create_vedio_driver(pdriver driver) {
    driver->init_handle = (initialize_handle_t)initialize_driver;
    driver->pc_handle = (pc_handle_t)process_center;
    driver->terminate_handle = (terminate_handle_t)terminate_driver;
    driver->state = DS_UNINITIALIZE;
}