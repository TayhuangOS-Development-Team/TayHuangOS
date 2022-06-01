#include <syscall/syscall.h>
#include <debug/logging.h>
#include <ipc/ipc.h>

void entry(void) {
    linfo ("DISK DRIVER!");
    char buffer[256] = {};
    recv_any_msg(buffer);
    linfo (buffer);
    while (1);
}