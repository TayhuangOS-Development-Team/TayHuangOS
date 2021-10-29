.PHONY: build
build:
	cd kernel ; make build

.PHONY: clean
clean:
	cd kernel ; make clean

.PHONY: image
image:
	cd kernel ; make image
	sudo umount /mnt/flyOS