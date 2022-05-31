OBJECTSDIR := $(OBJECTSDIR)/drivers/
BINDIR := $(BINDIR)/drivers/
DRIVERSDIR := $(ROOTDIR)/drivers
export OBJECTSDIR DRIVERSDIR

#编译
.PHONY: build
build:
	cd disk ; make build

#清理
.PHONY: clean
clean:
	cd disk ; make build

.PHONY: image
image:
	cd disk ; make image