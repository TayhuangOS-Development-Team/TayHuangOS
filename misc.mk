.PHONY: show_version, stat_code_density, doc

show_version:
	$(ECHO) "TayhuangOS Version: $(VERSION)"

stat_code_density:
	$(COMMENTS_STAT)

doc:
ifeq ($(BUILD_LOADER), true)
	$(CD) loader && $(MAKE) doc
endif

$(RAW_ICON): $(TAYHUANG_ICON)
	$(PNG_CONV) $(TAYHUANG_ICON) $(RAW_ICON)