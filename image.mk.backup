#写入映像
.PHONY: image
image:
	$(ECHO) "TayhuangOS Version: $(VERSION)"
	-$(MAKE) image_losetup_
	-$(SUDO) $(UMOUNT) $(TAYHUANGOS_MOUNT_DIR)
	-$(SUDO) $(LOOP_SETUP) -d $(LOOPB)

.PHONY: image_losetup_
image_losetup_:
	$(SUDO) $(LOOP_SETUP) $(LOOPB) $(TAYHUANGOS_IMG) -o 1048576
	$(SUDO) $(MOUNT) $(LOOPB) $(TAYHUANGOS_MOUNT_DIR)
	$(SUDO) $(COPY) ./configs/grub.cfg $(TAYHUANGOS_MOUNT_DIR)/boot/grub
ifeq ($(VBE_MODE), ENABLE)
	$(SUDO) $(COPY) $(RAW_ICON) $(TAYHUANGOS_MOUNT_DIR)/
endif

ifeq ($(BUILD_LOADER), true)
	$(CD) loader && $(MAKE) image
endif
ifeq ($(BUILD_KERNEL), true)
	$(CD) kernel && $(MAKE) image
endif
ifeq ($(BUILD_MODULE), true)
	$(CD) module && $(MAKE) image
endif