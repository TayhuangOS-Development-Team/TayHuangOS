ARCHITECTURE := x86_64 #架构
export ARCHITECTURE
ARCHDEF_C := -DARCH_$(ARCHITECTURE) #架构宏
export ARCHDEF_C

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
	if [ ! -d "/mnt/tayhuangOS" ];then \
		sudo mkdir /mnt/tayhuangOS; \
	else \
		echo "mount directory already created"; \
	fi;
	if [ ! -d "/mnt/tayhuangBoot" ];then \
		sudo mkdir /mnt/tayhuangBoot; \
	else \
		echo "mount directory already created"; \
	fi;
	bximage < ./setup/new_boot_img_input.txt
	bximage < ./setup/new_system_img_input.txt
	mkfs.msdos -F 32 ./tayhuangOS.img

#编译
.PHONY: build
build:
	cd arch/$(ARCHITECTURE)/ ; make build
	cd kernel ; make build

#清理
.PHONY: clean
clean:
	cd arch/$(ARCHITECTURE)/ ; make clean
	cd kernel ; make clean

#写入映像
.PHONY: image
image:
	cd arch/$(ARCHITECTURE)/ ; make image
	sudo umount /mnt/tayhuangBoot
	sudo mount -o loop tayhuangOS.img /mnt/tayhuangOS
	cd kernel ; make image
	sudo umount /mnt/tayhuangOS