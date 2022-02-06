ENTRY(_start)
OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH("i386:x86-64")

SECTIONS
{
    . = 0x120000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }
}