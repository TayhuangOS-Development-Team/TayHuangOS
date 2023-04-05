# #设置环境
# .PHONY: setup_workspace
# setup_workspace:
# 	if [ ! -d "$(TAYHUANGOS_MOUNT_DIR)" ];then \
# 		$(SUDO) $(MKDIR) $(TAYHUANGOS_MOUNT_DIR); \
# 	else \
# 		$(ECHO) "mount directory already created"; \
# 	fi;
# 	$(DD) if=/dev/zero of=$(TAYHUANGOS_IMG) bs=512 count=$(IMAGE_SECTORS)

# 	$(MKDIR) $(BUILDDIR)
# 	$(MKDIR) $(OBJECTSDIR)
# 	$(MKDIR) $(BINDIR)

# 	$(FDISK) $(TAYHUANGOS_IMG)
# 	-$(MAKE) setup_workspace_losetup_

# 	-$(SUDO) $(LOOP_SETUP) -d $(LOOPB)	#losetup LOOPA失败不会影响LOOPB
# 	$(SUDO) $(LOOP_SETUP) -d $(LOOPA)

# 	$(SUDO) $(CHMOD) +x $(PNG_CONV)
# 	$(SUDO) $(CHMOD) +x $(COUNTER)
# 	$(SUDO) $(CHMOD) +x $(COMMENTS_STAT)

# .PHONY: setup_workspace_losetup_
# setup_workspace_losetup_:
# 	$(SUDO) $(LOOP_SETUP) $(LOOPA) $(TAYHUANGOS_IMG)
# 	$(SUDO) $(LOOP_SETUP) $(LOOPB) $(TAYHUANGOS_IMG) -o $(KERNEL_PARTITION_OFFSET)
	
# 	$(SUDO) $(MKFS) $(LOOPB)
# 	$(SUDO) $(MOUNT) $(LOOPB) $(TAYHUANGOS_MOUNT_DIR)

# 	#TODO:添加UEFI支持
# 	-$(SUDO) $(GRUB_INSTALL) --target=i386-pc --root-directory=$(TAYHUANGOS_MOUNT_DIR) --no-floppy --modules="$(GRUB_MODULES)" $(LOOPA)

# 	$(SUDO) $(UMOUNT) $(TAYHUANGOS_MOUNT_DIR)