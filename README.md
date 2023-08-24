# English Version

# 英文版

<div align="center">
<a href="#">
<h1><img src="TayhuangOS.svg" alt="Logo" width="180" height="180"></h1>
</a>

### Tayhuang OS

</div>

> There was a person called Pangu. He borned in Tayhuang.

It's a OS made by me

If you want to download a operating system for use, please look for other repo.It has not be done.

Everyone is welcome to program OS with me, if you have some experience about it.

There's my QQ number: 2715626245

Or you can send an email to me, my email address is: song_of_the_fly@163.com


---

# Chinese Version

# 中文版


<div align="center">
<a href="#">
<h1><img src="TayhuangOS.svg" alt="Logo" width="180" height="180"></h1>
</a>

### 太荒 OS

</div>

> 粤自盘古，生于太荒。

这是我自制的OS

如果你想要下载一个操作系统来用，请去找别的repo,因为它还没做好

我欢迎任何人和我一起写OS,只要你有这方面的一点知识

QQ号: 2715626245

或者你可以给我发邮件, 地址: song_of_the_fly@163.com

---

# 设置工作环境的方法

在 https://github.com/TayhuangOS-Development-Team/TayHuangOS_tools 中下载开发工具包

再在工具包目录下输入source ./setup.sh

之后把~/opt/cross/bin加入你的环境变量PATH中

再输入x86_64-elf-gcc --version

若出现

```
86_64-elf-gcc (GCC) 9.4.0
Copyright (C) 2019 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

则成功

在项目目录下输入make setup_workspace

配置好镜像的分区与启动分区(启动分区(即OS所在的分区)作为第一个分区)后输入make build_and_run

如果成功启动就可以了

## "我不会配置镜像分区与启动分区的"这么做

在出现`命令(输入 m 获取帮助)：`后依次输入

```
n
p
1
2048
262143
a
w
```

即可
