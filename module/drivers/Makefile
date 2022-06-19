OBJECTSDIR := $(OBJECTSDIR)/drivers/
BINDIR := $(BINDIR)/drivers/
DRIVERSDIR := $(MODULEDIR)/drivers/
export OBJECTSDIR DRIVERSDIR

#编译
.PHONY: build
build:
	$(CHANGE_DIR) ./video/ ; $(MAKE) build
	$(CHANGE_DIR) ./keyboard/ ; $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CHANGE_DIR) ./video/ ; $(MAKE) clean
	$(CHANGE_DIR) ./keyboard/ ; $(MAKE) clean

.PHONY: image
image:
	$(CHANGE_DIR) ./video/ ; $(MAKE) image
	$(CHANGE_DIR) ./keyboard/ ; $(MAKE) image