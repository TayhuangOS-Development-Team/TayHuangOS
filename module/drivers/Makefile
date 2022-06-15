OBJECTSDIR := $(OBJECTSDIR)/drivers/
BINDIR := $(BINDIR)/drivers/
DRIVERSDIR := $(MODULEDIR)/drivers/
export OBJECTSDIR DRIVERSDIR

#编译
.PHONY: build
build:
	cd ./video/ ; make build
	cd ./keyboard/ ; make build

#清理
.PHONY: clean
clean:
	cd ./video/ ; make clean
	cd ./keyboard/ ; make clean

.PHONY: image
image:
	cd ./video/ ; make image
	cd ./keyboard/ ; make image