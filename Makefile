# 配置区

# ARCHITECTURE := x86_64 #架构
# FILESYSTEM := fat32
# MODE := debug
# QEMU_ARGS := 
# VBE_MODE := DISABLE

#定义区

ARCHITECTURE ?= x86_64
ARCHDEF_C := -DARCH_$(ARCHITECTURE) #架构宏
FILESYSTEM ?= fat32
MODE ?= debug
VBE_MODE ?= DISABLE

export ARCHITECTURE ARCHDEF_C MODE VBE_MODE

#目录区

ROOTDIR := $(shell pwd)
BUILDDIR := $(ROOTDIR)/build/$(MODE)/
BINDIR := $(BUILDDIR)/bin/
OBJECTSDIR := $(BUILDDIR)/objects/
TAYHUANGOS_MOUNT_DIR := /mnt/tayhuangOS
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
GRUB_INSTALL := grub-install
GRUB_MODULES := normal part_msdos ext2 fat multiboot all_video
SUDO := sudo
MOUNT := mount
UMOUNT := umount
ECHO := echo
CHANGE_DIR := cd
COPY := cp
OBJCOPY := objcopy
FDISK := fdisk
LOOP_SETUP := losetup

ifeq ($(ARCHITECTURE), x86_64)
	QEMU := qemu-system-x86_64
endif

QEMU_ARGS ?=
QEMU_ARGS += -hda tayhuangOS.img \
             -m 64m \
			 -serial stdio \
			 -rtc base=localtime \
			 -name "TayhuangOS"

QEMU_DEBUG_ARGS := $(QEMU_ARGS) -s -S

ifeq ($(FILESYSTEM), fat32)
	MKFS := $(_MKFS).msdos -F 32 -n "TayhuangOS" -v -f 2
endif

export ROOTDIR BUILDDIR BINDIR OBJECTSDIR TAYHUANGOS_MOUNT_DIR TAYHUANGBOOT_MOUNT_DIR
export GCC GPP ASM GAS RM MKDIR LD IMAGEGEN _MKFS FILESYSTEM MKFS GRUB_INSTALL SUDO MOUNT UMOUNT ECHO CHANGE_DIR COPY OBJCOPY FDISK


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
	$(IMAGEGEN) < ./setup/new_system_img_input.txt

	$(MKDIR) -v -p $(BUILDDIR)
	$(MKDIR) -v -p $(OBJECTSDIR)
	$(MKDIR) -v -p $(BINDIR)

	$(FDISK) $(TAYHUANGOS_IMG)

	$(SUDO) $(LOOP_SETUP) /dev/loop16 $(TAYHUANGOS_IMG)
	$(SUDO) $(LOOP_SETUP) /dev/loop17 $(TAYHUANGOS_IMG) -o 1048576

	$(SUDO) $(MKFS) /dev/loop17
	$(SUDO) $(MOUNT) /dev/loop17 $(TAYHUANGOS_MOUNT_DIR)

	$(SUDO) $(GRUB_INSTALL) --root-directory=$(TAYHUANGOS_MOUNT_DIR) --no-floppy --modules="$(GRUB_MODULES)" /dev/loop16

	$(SUDO) $(UMOUNT) $(TAYHUANGOS_MOUNT_DIR)

	$(SUDO) $(LOOP_SETUP) -d /dev/loop16
	$(SUDO) $(LOOP_SETUP) -d /dev/loop17


#编译
.PHONY: build
build:
	$(CHANGE_DIR) libs ; $(MAKE) build
	$(CHANGE_DIR) loader ; $(MAKE) build
	$(CHANGE_DIR) kernel ; $(MAKE) build
	$(CHANGE_DIR) module ; $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CHANGE_DIR) libs ; $(MAKE) clean
	$(CHANGE_DIR) loader ; $(MAKE) clean
	$(CHANGE_DIR) kernel ; $(MAKE) clean
	$(CHANGE_DIR) module ; $(MAKE) clean

#写入映像
.PHONY: image
image:
	$(SUDO) $(LOOP_SETUP) /dev/loop17 $(TAYHUANGOS_IMG) -o 1048576
	$(SUDO) $(MOUNT) /dev/loop17 $(TAYHUANGOS_MOUNT_DIR)

	$(SUDO) $(COPY) ./configs/grub.cfg $(TAYHUANGOS_MOUNT_DIR)/boot/grub

	$(CHANGE_DIR) loader ; $(MAKE) image
	$(CHANGE_DIR) kernel ; $(MAKE) image
	$(CHANGE_DIR) module ; $(MAKE) image

	$(SUDO) $(UMOUNT) $(TAYHUANGOS_MOUNT_DIR)
	$(SUDO) $(LOOP_SETUP) -d /dev/loop17

.PHONY: run
run:
	$(QEMU) $(QEMU_ARGS)

.PHONY: debug
debug:
	$(QEMU) $(QEMU_DEBUG_ARGS)

.PHONY: build_and_run
build_and_run: all run
	$(ECHO) "done"

.PHONY: build_and_debug
build_and_debug: all debug
	$(ECHO) "done"