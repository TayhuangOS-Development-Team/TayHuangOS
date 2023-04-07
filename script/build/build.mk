# 目标模式(内核/loader k 模块m 静态库s 动态库l)
mode ?=

# obj文件
objects ?=

# 目标
target ?=

# 库列表
libraries ?=

libraries-ld := $(foreach lib, $(libraries), -l$(lib))

# 源文件目录
dir-src ?=

# 目标文件目录
dir-obj ?=

build-objects := $(foreach obj, $(objects), $(dir-obj)/$(obj))

# 架构(x86 x86_64)
architecture ?= x86