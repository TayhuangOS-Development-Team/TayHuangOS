#include "drivers.h"
#include "./vedio/vedio_driver.h"
#include "devices.h"

driver_t vedio_driver;

PUBLIC void init_drivers(void) {
    create_vedio_driver(&vedio_driver);
    vedio_driver.init_handle(&vedio_device, &vedio_driver, alloc_id());
}

PUBLIC void terminate_drivers(void) {
    vedio_driver.terminate_handle(&vedio_device, &vedio_driver);
}