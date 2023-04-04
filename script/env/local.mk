include $(global-env)

path-f = $(path-e)/$(firstword $(MAKEFILE_LIST))
path-d = $(dir $(path-f))