# 标志位
flags-asm  ?=
defs-asm ?=
include-asm ?=

$(dir-obj)/%.o : $(dir-src)/%.S
	$(call prepare, $@)
	$(compiler-asm) -c -o $@ $(flags-asm) $(defs-asm) $(include-asm) $^