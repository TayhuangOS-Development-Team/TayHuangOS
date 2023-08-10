path-e = $(shell pwd)
path-script = $(path-e)/script/
path-tools := $(path-e)/tools/
path-mount := /mnt/tayhuangOS/
path-img := $(path-e)/tayhuangOS.img
path-lib := $(path-e)/libs/
path-include := $(path-e)/include/

#TODO
offset-kernel ?= 1048576

q ?= @