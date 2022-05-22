OBJECTSDIR := $(OBJECTSDIR)/libc/
export OBJECTSDIR

#编译
.PHONY: build
build:
	cd libc ; make build
	cd libkmod ; make build

#清理
.PHONY: clean
clean:
	cd libc ; make clean
	cd libkmod ; make build