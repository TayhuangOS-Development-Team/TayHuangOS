# 标志位
flags-c  ?=
defs-c ?=
include-c ?=

$(dir-obj)/%.o : $(dir-src)/%.c
	$(call prepare, $@)
	$(compiler-c) -c -o $@ $(flags-c) $(defs-c) $(include-c) $^