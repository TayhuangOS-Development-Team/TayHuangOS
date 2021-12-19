#include "driver.h"

PRIVATE id_t cur_id = 0;

PUBLIC id_t alloc_id() {
    return cur_id ++;
}