include $(path-script)/tool.mk

define prepare
	$(mkdir) -p -v $(dir $(1))
endef