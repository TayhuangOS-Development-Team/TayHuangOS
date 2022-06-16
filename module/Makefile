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
	cd vdd ; make build

#清理
.PHONY: clean
clean:
	cd drivers ; make clean
	cd setup ; make clean
	cd test ; make clean
	cd vdd ; make clean

.PHONY: image
image:
	cd drivers ; make image
	cd setup ; make image
	cd test ; make image
	cd vdd ; make image