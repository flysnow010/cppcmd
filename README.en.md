# 1 cppcmd概述

In embedded development, command line interaction system testing functionality is often utilized. This article describes a command line interaction system developed using C++language, which supports command registration (in C or C++format), command grouping, command execution, and displaying command prompts.
# 2 设计
![Classdiagram](https://img-blog.csdnimg.cn/direct/10084492b59a42cbadaf2dda20db7e76.png#pic_center)

## 2.1 CommandManager
Interface List:
- registerCommand Registration command
- unregisterCommand Unregister command
- showCommand Display single command information
- showCommands Display commands by group
- execCommand Execute commands
- returnValue Get command return value
- run Run the command-line system and exit with the default command exit

## 2.2 Command
Member List:
- cpp_handler C++ command handler
- c_hanlder   C command handler
- name Command Name
- help Command Help
- group Command Group

Interface List：
- empty Is the command empty
  
## 2.3 CommandHelper
Interface List:
- signal Is it an exit signal? The default is false
- doSingal Set exit signal
- add Add command
- show Display single command information
- showGroup Display commands by group
- addDefault Add default commands
  - ret Return information, and finally command the return value
  - print Print Information
  - test Test if the return value is correct
  - pause Pause pressing any key
  - exit quit
  - help Get help information
- parse Parse and execute commands

## 2.4 CmdIO
Interface List:
- readLine Read a line from the command line
- readAnyKey Read any key

