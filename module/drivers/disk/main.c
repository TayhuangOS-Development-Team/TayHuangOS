#include <syscall/syscall.h>
#include <tayhuang/io.h>
#include <tayhuang/ports.h>

void entry(void) {
    outb (SERIAL_SEND, 'D');
    outb (SERIAL_SEND, 'I');
    outb (SERIAL_SEND, 'S');
    outb (SERIAL_SEND, 'K');
    outb (SERIAL_SEND, '\n');
    while (1);
}