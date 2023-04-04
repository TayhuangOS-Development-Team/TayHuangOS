# 标志位
flags-asm  ?=
defs-asm ?=
include-asm ?=

compiler-asm ?= $(prefix-compiler)as

%.o : %.S
	$(compiler-asm) -o $@ $(flags-asm) $(defs-asm) $(include-asm) $^

$(warning $(compiler-asm))