# 1 cppcmd概述

在嵌入式开发中，经常会利用命令行交互系统测试功能，本文描述使用C++语言开发的一个命令行交互系统，支持命令注册(C或C++格式)，命令分组，执行命令，显示命令提示。
# 2 设计
![类图](https://img-blog.csdnimg.cn/direct/10084492b59a42cbadaf2dda20db7e76.png#pic_center)

## 2.1 CommandManager
接口列表:
- registerCommand 注册命令
- unregisterCommand 注销命令
- showCommand 显示单条命令信息
- showCommands 按组显示命令
- execCommand 执行命令
- returnValue 获取命令返回值
- run 运行命令行系统，通过默认命令exit退出

## 2.2 Command
成员列表:
- cpp_handler C++命令处理器
- c_hanlder   C命令处理器
- name 命令名称
- help 命令帮助
- group 命令组

接口列表：
- empty 命令是否为空
  
## 2.3 CommandHelper
接口列表:
- signal 是否是退出信号，默认是false
- doSingal 设置退出信号
- add 增加命令
- show 显示单条命令信息
- showGroup 按组显示命令
- addDefault 增加默认命令
  - ret 返回信息最后命令返回值
  - print 打印信息
  - test 测试返回值是否正确
  - pause 暂停按任意键
  - exit 退出
  - help 获取帮助信息
- parse 解析并执行命令

## 2.4 CmdIO
接口列表:
- readLine 从命令行上读取一行
- readAnyKey 读取任意按键

