define if_exist
	if [ ! -d $(1) ]; then \
		$(2); \
	else \
		$(3); \
	fi;
endef

define if_mkdir
	$(call if_exist, $(1), $(sudo) $(mkdir) -p -v $(1), $(echo) "directory already created")
endef

define prepare
	$(call if_mkdir, $(dir $(1)))
endef
