# 设置环境
.PHONY: setup_workspace
setup_workspace:
	$(call if_mkdir, $(path-mount))
	$(dd) if=/dev/zero of=$(path-img) bs=512 count=$(image-sectors)

	$(fdisk) $(path-img)
	-$(MAKE) setup_losetup

	-$(sudo) $(loop_setup) -d $(loop-b)
	$(sudo) $(loop_setup) -d $(loop-a)

	$(sudo) $(chmod) +x $(png-converter)
	$(sudo) $(chmod) +x $(comments-stat)

.PHONY: setup_losetup
setup_losetup:
	$(sudo) $(loop_setup) $(loop-a) $(path-img)
	$(sudo) $(loop_setup) $(loop-b) $(path-img) -o $(offset-kernel)
	
	$(sudo) $(mkfs-fs) $(loop-b)
	$(sudo) $(mount) $(loop-b) $(path-mount)

	#TODO:添加UEFI支持
	-$(sudo) $(grub-install-target) --root-directory=$(path-mount) --no-floppy --modules="$(grub-modules)" $(loop-a)

	$(sudo) $(umount) $(path-mount)