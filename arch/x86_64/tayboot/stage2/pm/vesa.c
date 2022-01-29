#include "vesa.h"
#include "../printf.h"

PUBLIC bool support_vesa(void) {
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    in_regs.eax = 0x4F00;
    in_regs.es = BASE_OF_DATA;
    in_regs.edi = 0;
    args.int_no = 0x10;
    intcall(&args);
    return ((word)out_regs.eax) == 0x004F;
}

PUBLIC void* enable_graphic(void) {
    if (! support_vesa())
        return NULL;
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    in_regs.eax = 0x4F02;
    in_regs.ebx = 0x4118;
    args.int_no = 0x10;
    intcall(&args);

    in_regs.eax = 0x4F01;
    in_regs.ecx = 0x0118;
    in_regs.es = BASE_OF_DATA;
    in_regs.edi = 0;
    args.int_no = 0x10;
    intcall(&args);

    stes(BASE_OF_DATA);
    void* addr = rdes32(40);
    return addr;
}