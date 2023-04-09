# 标志位
flags-c ?=
defs-c ?=
include-c ?=

$(dir-obj)/%.o : $(dir-src)/%.c
	$(call prepare, $@)
	$(q)$(compiler-c) -c -o $@ $(flags-c) $(defs-c) $(include-c) $^