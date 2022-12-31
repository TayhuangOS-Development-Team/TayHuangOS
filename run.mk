.PHONY: run, debug, build_and_run, build_and_debug

run:
	$(ECHO) "TayhuangOS Version: $(VERSION)"
	$(QEMU) $(QEMU_ARGS)

debug:
	$(ECHO) "TayhuangOS Version: $(VERSION)"
	$(QEMU) $(QEMU_DEBUG_ARGS)

build_and_run: all run
	$(ECHO) "done"

build_and_debug: all debug
	$(ECHO) "done"