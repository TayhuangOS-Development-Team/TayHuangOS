OBJECTSDIR := $(OBJECTSDIR)/test/
BINDIR := $(BINDIR)/test/
DRIVERSDIR := $(MODULEDIR)/test/
export OBJECTSDIR DRIVERSDIR

#编译
.PHONY: build
build:
	cd ./testbench1/ ; make build

#清理
.PHONY: clean
clean:
	cd ./testbench1/ ; make clean

.PHONY: image
image:
	cd ./testbench1/ ; make image