#include "multiboot2.h"

#define HEADER_LENGTH (16)

struct multiboot_header HEADER __attribute__((section(".multiboot"))) = {
    .magic    = MULTIBOOT2_HEADER_MAGIC,
    .architecture = MULTIBOOT_ARCHITECTURE_I386,
    .header_length = HEADER_LENGTH,
    .checksum = -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + HEADER_LENGTH),
};

void entry(void) {
    *(short*)(0xB8000) = 0x0F42;
    while (1);
}