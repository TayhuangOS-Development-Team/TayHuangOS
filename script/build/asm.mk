# 标志位
flags-asm ?=
defs-asm ?=
include-asm ?=

$(dir-obj)/%.o : $(dir-src)/%.S
	$(call prepare, $@)
	$(q)$(compiler-asm) -c -o $@ $(flags-asm) $(defs-asm) $(include-asm) $^