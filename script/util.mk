define extract
	$(foreach src, $(1), $(2)/$(src))
endef