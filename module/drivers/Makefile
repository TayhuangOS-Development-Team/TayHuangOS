OBJECTSDIR := $(OBJECTSDIR)/drivers/
BINDIR := $(BINDIR)/drivers/
DRIVERSDIR := $(MODULEDIR)/drivers
export OBJECTSDIR DRIVERSDIR

#编译
.PHONY: build
build:
	cd ./video/ ; make build
	echo "done!"

#清理
.PHONY: clean
clean:
	cd ./video/ ; make clean
	echo "done!"

.PHONY: image
image:
	cd ./video/ ; make image
	echo "done!"