# 1 概述
&emsp;&emsp;在嵌入式开发中，经常会利用命令行交互系统测试功能，本文描述使用C++语言开发的一个命令行交互系统，支持命令注册(C或C++格式)，命令分组，执行命令，显示命令提示。
# 4 实例
## 4.1 实例代码
```cpp
extern "C" int test1(int argc, char* argv[])
{
    for(int i = 0; i < argc; i++)
        std::cout << argv[i] << ",";
    std::cout << std::endl;
    return argc;
}

class CppcmdTest
{
    Cmd::CommandManager cmdManager;
public:
    int test2(Cmd::Args const& args)
    {
        for(auto arg = args.begin(); arg != args.end(); ++arg)
            std::cout << *arg << ",";
        std::cout << std::endl;

        return args.size();
    }

    void run()
    {
        cmdManager.registerCommand("test1", "test", test1, 
            "test1 arg1 arg2:"
            S"arg1:"
            SS"0x00 - the lane seq 0-1-2-3"
            SS"0x01 - the lane seq 0-1-3-2"
            SS"0x02 - the lane seq 0-2-1-3"
            SS"0x03 - the lane seq 0-2-3-1"
            SS"0x04 - the lane seq 0-3-1-2"
            SS"0x05 - the lane seq 0-3-2-1"
            NL
            S"arg2
            SS"0x06 - the lane seq 1-0-2-3"
            SS"0x07 - the lane seq 1-0-3-2"
            SS"0x08 - the lane seq 1-2-0-3"
            SS"0x09 - the lane seq 1-2-3-0"
            SS"0x0a - the lane seq 1-3-0-2"
            SS"0x0b - the lane seq 1-3-2-0"
        );
        
        cmdManager.registerCommand("test2", "test", HANDLER(CppCmdSuite::test2, this),  "this is a test");

        cmdManager.showCommands();
        cmdManager.run("Cmd->");
    }
};

int main(int argc, char *argv[])
{
    CppcmdTest cmd;
    cmd.run();
    return 0;
}
```
接口说明：
- test1 C格式handler函数
- test2 C++格式Handler函数
- run 注册命名，运行命令行系统
## 4.2 运行结果
```bash
test1 : bzlaneseq lane_seq(0x0-0x17): set jesd204b lane seq
         lane_seq:
          0x00 - jesd204b lane seq 0-1-2-3
          0x01 - jesd204b lane seq 0-1-3-2
          0x02 - jesd204b lane seq 0-2-1-3
          0x03 - jesd204b lane seq 0-2-3-1
          0x04 - jesd204b lane seq 0-3-1-2
          0x05 - jesd204b lane seq 0-3-2-1

          0x06 - jesd204b lane seq 1-0-2-3
          0x07 - jesd204b lane seq 1-0-3-2
          0x08 - jesd204b lane seq 1-2-0-3
          0x09 - jesd204b lane seq 1-2-3-0
          0x0a - jesd204b lane seq 1-3-0-2
          0x0b - jesd204b lane seq 1-3-2-0
test2 : this is a test

show 2 commands

Cmd->test1 this is a test1
this,is,a,test1,
Cmd->test2 this is a test2
this,is,a,test2,
Cmd->exit
```
