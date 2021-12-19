#include "drivers/drivers.h"
#include "drivers/devices.h"
#include "print.h"

void entry(void) {
    init_devices();
    init_drivers();
    clrscr();
    printStr("Hello, World!");
}