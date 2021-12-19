#include "devices.h"

device_t vedio_device;

PUBLIC void init_devices(void) {
    vedio_device.id = alloc_id();
    vedio_device.type = DT_VEDIO;
}