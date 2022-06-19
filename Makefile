# 配置区

# ARCHITECTURE := x86_64 #架构
# FILESYSTEM := msdos
# MODE := debug

#目录区

ROOTDIR := $(shell pwd)
BUILDDIR := $(ROOTDIR)/build/$(MODE)/
BINDIR := $(BUILDDIR)/bin/
OBJECTSDIR := $(BUILDDIR)/objects/
TAYHUANGOS_MOUNT_DIR := /mnt/tayhuangOS
TAYHUANGBOOT_MOUNT_DIR := /mnt/tayhuangBoot
TAYHUANGOS_IMG := tayhuangOS.img

#工具区

MKDIR := mkdir
GCC := gcc
GPP := g++
ASM := nasm
GAS := as
RM := rm
LD := ld
IMAGEGEN := bximage
_MKFS := mkfs
FILESYSTEM ?= msdos
MKFS := $(_MKFS).$(FILESYSTEM)
GRUB_INSTALL := grub-install
SUDO := sudo
MOUNT := mount
UMOUNT := umount
ECHO := echo
CHANGE_DIR := cd
COPY := cp
OBJCOPY := objcopy
FDISK := fdisk
LOOP_SETUP := losetup

export ROOTDIR BUILDDIR BINDIR OBJECTSDIR TAYHUANGOS_MOUNT_DIR TAYHUANGBOOT_MOUNT_DIR
export GCC GPP ASM GAS RM MKDIR LD IMAGEGEN _MKFS FILESYSTEM MKFS GRUB_INSTALL SUDO MOUNT UMOUNT ECHO CHANGE_DIR COPY OBJCOPY FDISK

#定义区

ARCHITECTURE ?= x86_64
ARCHDEF_C := -DARCH_$(ARCHITECTURE) #架构宏
MODE ?= debug

export ARCHITECTURE ARCHDEF_C MODE

#任务区

#编译并写入映像
.PHONY: all
all: build image
	$(ECHO) "done"

#重编译并写入映像
.PHONY: rebuild
rebuild: clean build image
	$(ECHO) "done"

#设置环境并构建
.PHONY: setup_and_build
setup_and_build: setup_workspace all
	$(ECHO) "done"

#设置环境
.PHONY: setup_workspace
setup_workspace:
	if [ ! -d "$(TAYHUANGOS_MOUNT_DIR)" ];then \
		$(SUDO) $(MKDIR) $(TAYHUANGOS_MOUNT_DIR); \
	else \
		$(ECHO) "mount directory already created"; \
	fi;
	# if [ ! -d "$(TAYHUANGBOOT_MOUNT_DIR)" ];then \
	# 	$(SUDO) $(MKDIR) $(TAYHUANGBOOT_MOUNT_DIR); \
	# else \
	# 	$(ECHO) "mount directory already created"; \
	# fi;
	# $(IMAGEGEN) < ./setup/new_boot_img_input.txt
	$(IMAGEGEN) < ./setup/new_system_img_input.txt

	$(MKDIR) -v -p $(BUILDDIR)
	$(MKDIR) -v -p $(OBJECTSDIR)
	$(MKDIR) -v -p $(BINDIR)

	$(FDISK) $(TAYHUANGOS_IMG) < ./setup/fdisk_img.txt

	$(SUDO) $(LOOP_SETUP) /dev/loop16 $(TAYHUANGOS_IMG)
	$(SUDO) $(LOOP_SETUP) /dev/loop17 $(TAYHUANGOS_IMG) -o 1048576

	$(SUDO) $(MKFS) -F 32 /dev/loop17
	$(SUDO) $(MOUNT) /dev/loop17 $(TAYHUANGOS_MOUNT_DIR)

	$(SUDO) $(GRUB_INSTALL) --root-directory=$(TAYHUANGOS_MOUNT_DIR) --no-floppy --modules="normal part_msdos ext2 multiboot" /dev/loop16

	$(SUDO) $(UMOUNT) $(TAYHUANGOS_MOUNT_DIR)

	$(SUDO) $(LOOP_SETUP) -d /dev/loop16
	$(SUDO) $(LOOP_SETUP) -d /dev/loop17


#编译
.PHONY: build
build:
	#$(CHANGE_DIR) arch/$(ARCHITECTURE)/ ; $(MAKE) build
	$(CHANGE_DIR) libs ; $(MAKE) build
	#$(CHANGE_DIR) kernel ; $(MAKE) build
	$(CHANGE_DIR) module ; $(MAKE) build

#清理
.PHONY: clean
clean:
	# $(CHANGE_DIR) arch/$(ARCHITECTURE)/ ; $(MAKE) clean
	$(CHANGE_DIR) libs ; $(MAKE) clean
	# $(CHANGE_DIR) kernel ; $(MAKE) clean
	$(CHANGE_DIR) module ; $(MAKE) clean

#写入映像
.PHONY: image
image:
	# $(CHANGE_DIR) arch/$(ARCHITECTURE)/ ; $(MAKE) image
	# $(SUDO) $(UMOUNT) $(TAYHUANGBOOT_MOUNT_DIR)
	$(SUDO) $(LOOP_SETUP) /dev/loop17 $(TAYHUANGOS_IMG) -o 1048576
	$(SUDO) $(MOUNT) /dev/loop17 $(TAYHUANGOS_MOUNT_DIR)
	# $(CHANGE_DIR) kernel ; $(MAKE) image
	$(CHANGE_DIR) module ; $(MAKE) image
	$(SUDO) $(UMOUNT) $(TAYHUANGOS_MOUNT_DIR)
	$(SUDO) $(LOOP_SETUP) -d /dev/loop17