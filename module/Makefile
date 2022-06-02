OBJECTSDIR := $(OBJECTSDIR)/module/
BINDIR := $(BINDIR)/module/
MODULEDIR := $(ROOTDIR)/module
export OBJECTSDIR MODULEDIR

#编译
.PHONY: build
build:
	cd drivers ; make build
	cd setup ; make build

#清理
.PHONY: clean
clean:
	cd drivers ; make build
	cd setup ; make clean

.PHONY: image
image:
	cd drivers ; make image
	cd setup ; make image
