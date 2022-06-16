OBJECTSDIR := $(OBJECTSDIR)/libs/
export OBJECTSDIR

#编译
.PHONY: build
build:
	cd libc ; make build
	cd libkmod ; make build
	cd libfifo ; make build

#清理
.PHONY: clean
clean:
	cd libc ; make clean
	cd libkmod ; make clean
	cd libfifo ; make clean