OBJECTSDIR := $(OBJECTSDIR)/test/
BINDIR := $(BINDIR)/test/
DRIVERSDIR := $(MODULEDIR)/test/
export OBJECTSDIR DRIVERSDIR

#编译
.PHONY: build
build:
	$(CHANGE_DIR) ./testbench1/ ; $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CHANGE_DIR) ./testbench1/ ; $(MAKE) clean

.PHONY: image
image:
	$(CHANGE_DIR) ./testbench1/ ; $(MAKE) image