ifeq ($(architecture), x86)
prefix-compiler ?= x86_64-elf-
else ifeq  ($(architecture), x86_64)
prefix-compiler ?= x86_64-elf-
endif

mkdir ?= mkdir

define prepare
	$(mkdir) $(dir $(1))
endef

$(warning $(prefix-compiler))	