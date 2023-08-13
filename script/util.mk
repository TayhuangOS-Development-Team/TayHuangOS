define if_exist
	$(q)if [ ! -d $(1) ]; then \
		$(2); \
	fi;
endef

define if_mkdir
	$(call if_exist, $(1), $(mkdir) -p -v $(1))
endef

define if_sudo_mkdir
	$(call if_exist, $(1), $(sudo) $(mkdir) -p -v $(1))
endef

define prepare
	$(call if_mkdir, $(dir $(1)))
endef
