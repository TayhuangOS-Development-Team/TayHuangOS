# 标志位
flags-ar ?= rcs

define ar-link
	$(q)$(compiler-ar) $(flags-ar) $(1) $(2)
endef