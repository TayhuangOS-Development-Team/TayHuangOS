OBJECTSDIR := $(OBJECTSDIR)/module/
BINDIR := $(BINDIR)/module/
MODULEDIR := $(ROOTDIR)/module
export OBJECTSDIR MODULEDIR

#编译
.PHONY: build
build:
	cd drivers ; make build
	cd setup ; make build
	cd mm ; make build
	cd test ; make build

#清理
.PHONY: clean
clean:
	cd drivers ; make build
	cd setup ; make clean
	cd mm ; make clean
	cd test ; make clean

.PHONY: image
image:
	cd drivers ; make image
	cd setup ; make image
	cd mm ; make image
	cd test ; make image