override flags-c += -m64 -DBITS=64
override flags-asm += --64

prefix-compiler ?= x86_64-elf-

$(warning "ARCH=x86_64")