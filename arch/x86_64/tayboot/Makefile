IMG := $(ROOTDIR)/tayhuangBoot.img #镜像文件
IMG_MNT := /mnt/tayhuangBoot/ #挂载点
export IMG_MNT
BOOT_OUTPUT := $(BINDIR)/boot.bin #BOOT输出
LOADER_OUTPUT := $(BINDIR)/loader.bin #Loader输出
DD := dd #写磁盘工具
ASM := nasm #汇编器
RM := rm #删除工具
GCC := gcc #C编译器
TAYBOOT_ROOT := $(shell pwd)
export TAYBOOT_ROOT
OBJECTSDIR := $(OBJECTSDIR)/tayboot/
export OBJECTSDIR

export ROOTDIR MODE BUILDDIR BINDIR OBJECTSDIR

#构建
.PHONY: build
build:
	sudo mkdir -p -v $(dir $(BOOT_OUTPUT))
	$(ASM) boot.asm -o $(BOOT_OUTPUT)
	$(ASM) loader.asm -o $(LOADER_OUTPUT)
	cd stage2 ; make build
	cd stage3 ; make build

#清理
.PHONY: clean
clean:
	${RM} $(BOOT_OUTPUT) $(LOADER_OUTPUT)
	cd stage2 ; make clean
	cd stage3 ; make clean

#写入镜像
.PHONY: image
image:
	sudo ${DD} if=$(BOOT_OUTPUT) of=$(IMG) bs=512 conv=notrunc
	sudo mount -o loop $(IMG) $(IMG_MNT)
	sudo cp $(LOADER_OUTPUT) $(IMG_MNT)
	cd stage2 ; make image
	cd stage3 ; make image