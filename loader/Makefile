OBJECTSDIR := $(OBJECTSDIR)/loader/
BINDIR := $(BINDIR)/loader/
LOADERDIR := $(ROOTDIR)/loader/
export OBJECTSDIR LOADERDIR

#编译
.PHONY: build
build:
	$(CHANGE_DIR) grub_loader ; $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CHANGE_DIR) grub_loader ; $(MAKE) clean

.PHONY: image
image:
	$(CHANGE_DIR) grub_loader ; $(MAKE) image