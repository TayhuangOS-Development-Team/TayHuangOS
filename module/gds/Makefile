OBJECTSDIR := $(OBJECTSDIR)/gds/
BINDIR := $(BINDIR)/gds/
GDSDIR := $(ROOTDIR)/gds/
export OBJECTSDIR GDSDIR

#编译
.PHONY: build
build:
	cd gtty ; make build

#清理
.PHONY: clean
clean:
	cd gtty ; make clean

.PHONY: image
image:
	cd gtty ; make image