OBJECTSDIR := $(OBJECTSDIR)/libs/
export OBJECTSDIR

#编译
.PHONY: build
build:
	$(CHANGE_DIR) libc ; $(MAKE) build
	$(CHANGE_DIR) libkmod ; $(MAKE) build
	$(CHANGE_DIR) libfifo ; $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CHANGE_DIR) libc ; $(MAKE) clean
	$(CHANGE_DIR) libkmod ; $(MAKE) clean
	$(CHANGE_DIR) libfifo ; $(MAKE) clean