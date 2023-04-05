mkdir ?= mkdir

define prepare
	$(mkdir) -p -v $(dir $(1))
endef