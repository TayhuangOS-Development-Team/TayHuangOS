OBJECTSDIR := $(OBJECTSDIR)/module/
BINDIR := $(BINDIR)/module/
MODULEDIR := $(ROOTDIR)/module/
export OBJECTSDIR MODULEDIR

#编译
.PHONY: build
build:
	cd drivers ; make build
	cd setup ; make build
	cd test ; make build
	cd gds ; make build

#清理
.PHONY: clean
clean:
	cd drivers ; make build
	cd setup ; make clean
	cd test ; make clean
	cd gds ; make clean

.PHONY: image
image:
	cd drivers ; make image
	cd setup ; make image
	cd test ; make image
	cd gds ; make image