OBJECTSDIR := $(OBJECTSDIR)/vdd/
BINDIR := $(BINDIR)/vdd/
VDDDIR := $(ROOTDIR)/vdd/
export OBJECTSDIR VDDDIR

#编译
.PHONY: build
build:
	cd tty ; make build

#清理
.PHONY: clean
clean:
	cd tty ; make clean

.PHONY: image
image:
	cd tty ; make image