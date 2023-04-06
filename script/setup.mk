# 设置环境
.PHONY: setup-workspace
setup-workspace:
	$(call if_mkdir, $(path-mount))
	$(dd) if=/dev/zero of=$(path-img) bs=512 count=$(image-sectors)

	$(fdisk) $(path-img)
	-$(MAKE) setup-losetup

	-$(sudo) $(loop-setup) -d $(loop-b)
	$(sudo) $(loop-setup) -d $(loop-a)

	$(sudo) $(chmod) +x $(png-converter)
	$(sudo) $(chmod) +x $(comments-stat)

.PHONY: setup-losetup
setup-losetup:
	$(sudo) $(loop-setup) $(loop-a) $(path-img)
	$(sudo) $(loop-setup) $(loop-b) $(path-img) -o $(offset-kernel)
	
	$(sudo) $(mkfs-fs) $(loop-b)
	$(sudo) $(mount) $(loop-b) $(path-mount)

	#TODO:添加UEFI支持
	-$(sudo) $(grub-install-target) --root-directory=$(path-mount) --no-floppy --modules="$(grub-modules)" $(loop-a)

	$(sudo) $(umount) $(path-mount)