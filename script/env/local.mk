include $(global-env)

path-f = $(firstword $(MAKEFILE_LIST))
path-d = $(dir $(path-f))