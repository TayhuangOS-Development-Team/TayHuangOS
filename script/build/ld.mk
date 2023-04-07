# 标志位
flags-ld  ?=
libraries-ld ?=
script-ld ?=

define ld-link
	$(compiler-ld)  -L"$(path-e)/build/bin/libs/$(architecture)/" $(flags-ld) -T $(script-ld) -o $(1) $(2) $(libraries-ld)
endef