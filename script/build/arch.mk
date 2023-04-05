ifeq ($(architecture), x86)
arch-include := x86.mk
else
ifeq ($(architecture), x86_64)
arch-include := x86_64.mk
endif
endif

arch-include ?= x86.mk

defs-c += -D__ARCHITECTURE__=$(architecture)

include $(path-script)/build/arch/$(arch-include)