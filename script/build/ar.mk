# 标志位
flags-ar ?= rcs

define ar-link
	$(compiler-ar) $(flags-ar) $(1) $(2)
endef