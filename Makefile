ARCHITECTURE := x86_64
export ARCHITECTURE
ARCHDEF_C := -DARCH_$(ARCHITECTURE)
export ARCHDEF_C

.PHONY: all
all: build image
	echo "done"

.PHONY: rebuild
rebuild: clean build image
	echo "done"

.PHONY: setup_and_build
setup_and_build: setup_workspace all
	echo "done"

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
	mkfs.msdos ./tayhuangOS.img

.PHONY: build
build:
	cd arch/$(ARCHITECTURE)/ ; make build
#	cd kernel ; make build

.PHONY: clean
clean:
	cd arch/$(ARCHITECTURE)/ ; make clean
#	cd kernel ; make clean

.PHONY: image
image:
	cd arch/$(ARCHITECTURE)/ ; make image
	sudo umount /mnt/tayhuangBoot
#	sudo mount -o loop tayhuangOS.img /mnt/tayhuangOS
#	cd kernel ; make image
#	sudo umount /mnt/tayhuangOS