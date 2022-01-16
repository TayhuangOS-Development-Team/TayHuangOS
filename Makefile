ARCHITECTURE := x86_64
export ARCHITECTURE

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
	bximage < new_img_input.txt

.PHONY: build
build:
	cd arch/$(ARCHITECTURE)/ ; make build
	cd kernel ; make build

.PHONY: clean
clean:
	cd arch/$(ARCHITECTURE)/ ; make clean
	cd kernel ; make clean

.PHONY: image
image:
	cd arch/$(ARCHITECTURE)/ ; make image
	cd kernel ; make image
	sudo umount /mnt/tayhuangOS