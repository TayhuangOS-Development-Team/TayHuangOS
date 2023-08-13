# 设置环境
.PHONY: setup-workspace
setup-workspace:
	$(q)$(call if_sudo_mkdir, $(path-mount))
	$(q)$(dd) if=/dev/zero of=$(path-img) bs=512 count=$(image-sectors)

	$(q)$(fdisk) $(path-img)
	-$(q)$(MAKE) setup-losetup

	-$(q)$(sudo) $(loop-setup) -d $(loop-b)
	$(q)$(sudo) $(loop-setup) -d $(loop-a)

	$(q)$(sudo) $(chmod) +x $(png-converter)
	$(q)$(sudo) $(chmod) +x $(comments-stat)

.PHONY: setup-losetup
setup-losetup:
	$(q)$(sudo) $(loop-setup) $(loop-a) $(path-img)
	$(q)$(sudo) $(loop-setup) $(loop-b) $(path-img) -o $(offset-kernel)
	
	$(q)$(sudo) $(mkfs-fs) $(loop-b)
	$(q)$(sudo) $(mount) $(loop-b) $(path-mount)

	#TODO:添加UEFI支持
	-$(q)$(sudo) $(grub-install-target) --root-directory=$(path-mount) --no-floppy --modules="$(grub-modules)" $(loop-a)
	$(q)$(call if_sudo_mkdir, $(path-mount)/TayhuangOS/System/)

	$(q)$(sudo) $(umount) $(path-mount)