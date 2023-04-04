# 目标模式(内核k 模块m 静态库s 动态库l)
mode :=

# obj文件
objects :=

# 目标
targets :=

# 库列表
libraries :=

# 架构(x86 x86_64)
architecture ?= x86

# 架构特殊标志
ifeq ($(architecture), x86)
c_flags += -m32 -mregparm=1 -mpreferred-stack-boundary=4 -march=i386 -mfpmath=387
else ifeq  ($(architecture), x86_64)
c_flags += -m64
endif

$(warning $(c_flags))