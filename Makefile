ARCHITECTURE := x86_64 #架构
ARCHDEF_C := -DARCH_$(ARCHITECTURE) #架构宏

export ARCHITECTURE
export ARCHDEF_C

ROOTDIR := $(shell pwd)
MODE ?= debug
BUILDDIR := $(ROOTDIR)/build/$(MODE)/
BINDIR := $(BUILDDIR)/bin/
OBJECTSDIR := $(BUILDDIR)/objects/
TAYHUANGOS_MOUNT_DIR := /mnt/tayhuangOS
TAYHUANGBOOT_MOUNT_DIR := /mnt/tayhuangBoot
export ROOTDIR MODE BUILDDIR BINDIR OBJECTSDIR TAYHUANGOS_MOUNT_DIR TAYHUANGBOOT_MOUNT_DIR

#编译并写入映像
.PHONY: all
all: build image
	echo "done"

#重编译并写入映像
.PHONY: rebuild
rebuild: clean build image
	echo "done"

#设置环境并构建
.PHONY: setup_and_build
setup_and_build: setup_workspace all
	echo "done"

#设置环境
.PHONY: setup_workspace
setup_workspace:
	if [ ! -d "$(TAYHUANGOS_MOUNT_DIR)" ];then \
		sudo mkdir $(TAYHUANGOS_MOUNT_DIR); \
	else \
		echo "mount directory already created"; \
	fi;
	if [ ! -d "$(TAYHUANGBOOT_MOUNT_DIR)" ];then \
		sudo mkdir $(TAYHUANGBOOT_MOUNT_DIR); \
	else \
		echo "mount directory already created"; \
	fi;
	bximage < ./setup/new_boot_img_input.txt
	bximage < ./setup/new_system_img_input.txt
	mkfs.msdos -F 32 ./tayhuangOS.img
	mkdir -v -p $(BUILDDIR)
	mkdir -v -p $(OBJECTSDIR)
	mkdir -v -p $(BINDIR)

#编译
.PHONY: build
build:
	cd arch/$(ARCHITECTURE)/ ; make build
	cd libs ; make build
	cd kernel ; make build
	cd module ; make build

#清理
.PHONY: clean
clean:
	cd arch/$(ARCHITECTURE)/ ; make clean
	cd libs ; make clean
	cd kernel ; make clean
	cd module ; make clean

#写入映像
.PHONY: image
image:
	cd arch/$(ARCHITECTURE)/ ; make image
	sudo umount $(TAYHUANGBOOT_MOUNT_DIR)
	sudo mount -o loop tayhuangOS.img $(TAYHUANGOS_MOUNT_DIR)
	cd kernel ; make image
	cd module ; make image
	sudo umount $(TAYHUANGOS_MOUNT_DIR)

#debug
.PHONY: debug
debug:
	qemu-system-x86_64 -hda tayhuangBoot.img -hdc tayhuangOS.img -m 64 -serial stdio -s -S

#run
.PHONY: run
run:
	qemu-system-x86_64 -hda tayhuangBoot.img -hdc tayhuangOS.img -m 64 -serial stdio