OBJECTSDIR := $(OBJECTSDIR)/vdd/
BINDIR := $(BINDIR)/vdd/
VDDDIR := $(ROOTDIR)/vdd/
export OBJECTSDIR VDDDIR

#编译
.PHONY: build
build:
	$(CHANGE_DIR) tty ; $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CHANGE_DIR) tty ; $(MAKE) clean

.PHONY: image
image:
	$(CHANGE_DIR) tty ; $(MAKE) image