# 标志位
flags-ld  ?=
libraries-ld ?=
script-ld ?=

define ld-link
	$(compiler-ld) $(flags-ld) -T $(script-ld) -o $(1) $(2) --start-group $(libraries-ld) --end-group
endef