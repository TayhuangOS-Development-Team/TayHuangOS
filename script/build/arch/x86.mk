override flags-c += -m32 -mregparm=1 -mpreferred-stack-boundary=2 -march=i386 -mfpmath=387 -mno-mmx -mno-sse

prefix-compiler ?= x86_64-elf-

$(warning "ARCH=x86")