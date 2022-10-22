MODULENAME := loader

include $(SCRIPTDIR)/module.mk

LOADERDIR := $(CURDIR)
export LOADERDIR

#编译
build:
	$(call buildmod, grub)

#镜像
image:
	$(call imagemod, grub)

#清空
clean:
	$(call cleanmod, grub)