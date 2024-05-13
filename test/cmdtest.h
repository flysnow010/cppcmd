#ifndef _CMD_TEST_H_
#define _CMD_TEST_H_
#include <cpptest/cpptest.h>
#include <cppcmd.h>
#include <iostream>
extern "C" int test1(int argc, char* argv[]);

class CppCmdSuite : public Test::Suite
{
    Cmd::CommandManager cmdManager;
public:
	CppCmdSuite()
	{
        // TEST_ADD(CppCmdSuite::command_size)
        // TEST_ADD(CppCmdSuite::add_by_nullptr)
        // TEST_ADD(CppCmdSuite::add_by_nullstr)
        // TEST_ADD(CppCmdSuite::add_by_nullhanlder)
        // TEST_ADD(CppCmdSuite::add_by_nullcmd)
        // TEST_ADD(CppCmdSuite::add_by_nullhelp)
        // TEST_ADD(CppCmdSuite::remove_null)
        // TEST_ADD(CppCmdSuite::remove_cmd)
        // TEST_ADD(CppCmdSuite::add_by_nullgroup)
        // TEST_ADD(CppCmdSuite::add_normal)
        // TEST_ADD(CppCmdSuite::show_null)
        // TEST_ADD(CppCmdSuite::show_unadd)
        // TEST_ADD(CppCmdSuite::show_normal)
        // TEST_ADD(CppCmdSuite::show_null_commands)
        // TEST_ADD(CppCmdSuite::show_default_commands)
        // TEST_ADD(CppCmdSuite::show_invalid_group)
        // TEST_ADD(CppCmdSuite::show_group_commands)
        // TEST_ADD(CppCmdSuite::exec_null)
        // TEST_ADD(CppCmdSuite::exec_invalid)
        // TEST_ADD(CppCmdSuite::exec_normal)
        // TEST_ADD(CppCmdSuite::test_retrun_value)
        TEST_ADD(CppCmdSuite::success)
	}
	
private:
    void command_size()
    {
        TEST_ASSERT(cmdManager.commandSize() == 0)
    }
    void add_by_nullptr()
    {
        TEST_ASSERT(cmdManager.registerCommand(nullptr, Cmd::Handler(), nullptr) == false);
        TEST_ASSERT(cmdManager.registerCommand(nullptr, nullptr, Cmd::Handler(), nullptr) == false);
    }
    void add_by_nullstr()
    {
        TEST_ASSERT(cmdManager.registerCommand("", Cmd::Handler(), "") == false);
        TEST_ASSERT(cmdManager.registerCommand("", "", Cmd::Handler(), "") == false);
    }

    void add_by_nullhanlder()
    {
        TEST_ASSERT(cmdManager.registerCommand("test1", Cmd::Handler(), "this is a test1") == false);
        TEST_ASSERT(cmdManager.registerCommand("test2", "test", Cmd::Handler(), "this is a test1") == false);
    }

    void add_by_nullcmd()
    {
        TEST_ASSERT(cmdManager.registerCommand("", test1, "this is a test1") == false);
        TEST_ASSERT(cmdManager.registerCommand("", HANDLER(CppCmdSuite::test2, this), "this is a test1") == false);
    }

    void add_by_nullhelp()
    {
        int commandSize = cmdManager.commandSize();
        TEST_ASSERT(cmdManager.registerCommand("test1", test1, ""));
        TEST_ASSERT(cmdManager.registerCommand("test2", HANDLER(CppCmdSuite::test2, this), ""));
        TEST_ASSERT(cmdManager.commandSize() == commandSize + 2)
    }

    void remove_null()
    {
        TEST_ASSERT(cmdManager.unregisterCommand(nullptr) == false)
        TEST_ASSERT(cmdManager.unregisterCommand("") == false)
    }

    void remove_cmd()
    {
        int commandSize = cmdManager.commandSize();
        TEST_ASSERT(cmdManager.unregisterCommand("test1"))
        TEST_ASSERT(cmdManager.unregisterCommand("test2"))
        TEST_ASSERT(cmdManager.commandSize() == commandSize - 2)
    }

    void add_by_nullgroup()
    {
        int commandSize = cmdManager.commandSize();
        TEST_ASSERT(cmdManager.registerCommand("test1", test1, "this is test1"));
        TEST_ASSERT(cmdManager.registerCommand("test2", HANDLER(CppCmdSuite::test2, this), "this is test2"));
        TEST_ASSERT(cmdManager.commandSize() == commandSize + 2)

        commandSize = cmdManager.commandSize();
        TEST_ASSERT(cmdManager.unregisterCommand("test1"))
        TEST_ASSERT(cmdManager.unregisterCommand("test2"))
        TEST_ASSERT(cmdManager.commandSize() == commandSize - 2)
    }

    void add_normal()
    {
        int commandSize = cmdManager.commandSize();

        TEST_ASSERT(cmdManager.registerCommand("test1", "group", test1, "this is test1"));
        TEST_ASSERT(cmdManager.registerCommand("test2", "group", HANDLER(CppCmdSuite::test2, this), "this is test2"))
        TEST_ASSERT(cmdManager.commandSize() == commandSize + 2)

        commandSize = cmdManager.commandSize();
        TEST_ASSERT(cmdManager.unregisterCommand("test1"))
        TEST_ASSERT(cmdManager.unregisterCommand("test2"))
        TEST_ASSERT(cmdManager.commandSize() == commandSize - 2)
    }

    void show_null()
    {
        TEST_ASSERT(cmdManager.showCommand(nullptr) == false)
        TEST_ASSERT(cmdManager.showCommand("") == false)
    }

    void show_unadd()
    {
        TEST_ASSERT(cmdManager.showCommand("test1") == false)
        TEST_ASSERT(cmdManager.showCommand("test2") == false)
    }

    void show_normal()
    {
        TEST_ASSERT(cmdManager.registerCommand("test1", "group", test1, "this is test1"));
        TEST_ASSERT(cmdManager.registerCommand("test2", "group", HANDLER(CppCmdSuite::test2, this), "this is test2"))
        TEST_ASSERT(cmdManager.showCommand("test1"))
        TEST_ASSERT(cmdManager.showCommand("test2"))
        TEST_ASSERT(cmdManager.unregisterCommand("test1"))
        TEST_ASSERT(cmdManager.unregisterCommand("test2"))
    }

    void show_null_commands()
    {
        TEST_ASSERT(cmdManager.showCommands() == 0)
        TEST_ASSERT(cmdManager.showCommands(nullptr) == 0)
        TEST_ASSERT(cmdManager.showCommands("") == 0)
    }

    void show_default_commands()
    {
        TEST_ASSERT(cmdManager.registerCommand("test1", "group", test1, "this is test1"));
        TEST_ASSERT(cmdManager.registerCommand("test2", "group", HANDLER(CppCmdSuite::test2, this), "this is test2"))
        TEST_ASSERT(cmdManager.showCommands() == 2)
        TEST_ASSERT(cmdManager.unregisterCommand("test1"))
        TEST_ASSERT(cmdManager.unregisterCommand("test2"))
    }

    void show_invalid_group()
    {
        TEST_ASSERT(cmdManager.registerCommand("test1", "group", test1, "this is test1"));
        TEST_ASSERT(cmdManager.registerCommand("test2", "group", HANDLER(CppCmdSuite::test2, this), "this is test2"))
        TEST_ASSERT(cmdManager.showCommands("group1") == 0)
        TEST_ASSERT(cmdManager.unregisterCommand("test1"))
        TEST_ASSERT(cmdManager.unregisterCommand("test2"))
    }

    void show_group_commands()
    {
        TEST_ASSERT(cmdManager.registerCommand("test1", "group", test1, "this is test1"));
        TEST_ASSERT(cmdManager.registerCommand("test2", "group", HANDLER(CppCmdSuite::test2, this), "this is test2"))
        TEST_ASSERT(cmdManager.showCommands("group") == 2)
        TEST_ASSERT(cmdManager.unregisterCommand("test1"))
        TEST_ASSERT(cmdManager.unregisterCommand("test2"))
    }

    void exec_null()
    {
        TEST_ASSERT(cmdManager.execCommand(nullptr) == false)
        TEST_ASSERT(cmdManager.execCommand("") == false)
    }

    void exec_invalid()
    {
        TEST_ASSERT(cmdManager.execCommand("test1 this is a test") == false)
    }

    void exec_normal()
    {
        TEST_ASSERT(cmdManager.registerCommand("test1", "group", test1, "this is test1"));
        TEST_ASSERT(cmdManager.registerCommand("test2", "group", HANDLER(CppCmdSuite::test2, this), "this is test2"))
        TEST_ASSERT(cmdManager.execCommand("test1 this is a test"))
        TEST_ASSERT(cmdManager.execCommand("test2 this is a test"))
        TEST_ASSERT(cmdManager.unregisterCommand("test1"))
        TEST_ASSERT(cmdManager.unregisterCommand("test2"))
    }

    void test_retrun_value()
    {
        TEST_ASSERT(cmdManager.registerCommand("test1", "group", test1, "this is test1"));
        TEST_ASSERT(cmdManager.registerCommand("test2", "group", HANDLER(CppCmdSuite::test2, this), "this is test2"))
        TEST_ASSERT(cmdManager.execCommand("test1 arg1 arg2 arg3 arg4"))
        TEST_ASSERT(cmdManager.retrunValue() == 4)
        TEST_ASSERT(cmdManager.execCommand("test2 arg1 arg2 arg3 'this   is   arg4'"))
        TEST_ASSERT(cmdManager.retrunValue() == 4)
        TEST_ASSERT(cmdManager.execCommand("test2 arg1 arg2  'this   is   arg3' arg4"))
        TEST_ASSERT(cmdManager.retrunValue() == 4)
        TEST_ASSERT(cmdManager.execCommand("test2 arg1 arg2 arg3 \"this   is   arg4\""))
        TEST_ASSERT(cmdManager.retrunValue() == 4)
        TEST_ASSERT(cmdManager.execCommand("test2 arg1 arg2 arg3 \"this   is not  arg4'"))
        TEST_ASSERT(cmdManager.retrunValue() == 7)
        TEST_ASSERT(cmdManager.execCommand("test2 arg1 arg2 arg3 \"this   is not arg4"))
        TEST_ASSERT(cmdManager.retrunValue() == 7)
        TEST_ASSERT(cmdManager.execCommand("test2 arg1 arg2 arg3 \"this   is not arg4' 'this is arg8'"))
        TEST_ASSERT(cmdManager.retrunValue() == 8)
        TEST_ASSERT(cmdManager.unregisterCommand("test1"))
        TEST_ASSERT(cmdManager.unregisterCommand("test2"))
    }
    
	void success() {
        
        cmdManager.registerCommand("test1", "test", test1, 
            "bzlaneseq lane_seq(0x0-0x17): set jesd204b lane seq"
            S"lane_seq:"
            SS"0x00 - jesd204b lane seq 0-1-2-3"
            SS"0x01 - jesd204b lane seq 0-1-3-2"
            SS"0x02 - jesd204b lane seq 0-2-1-3"
            SS"0x03 - jesd204b lane seq 0-2-3-1"
            SS"0x04 - jesd204b lane seq 0-3-1-2"
            SS"0x05 - jesd204b lane seq 0-3-2-1"
            NL
            SS"0x06 - jesd204b lane seq 1-0-2-3"
            SS"0x07 - jesd204b lane seq 1-0-3-2"
            SS"0x08 - jesd204b lane seq 1-2-0-3"
            SS"0x09 - jesd204b lane seq 1-2-3-0"
            SS"0x0a - jesd204b lane seq 1-3-0-2"
            SS"0x0b - jesd204b lane seq 1-3-2-0"
        );
        
        cmdManager.registerCommand("test2", "test", HANDLER(CppCmdSuite::test2, this), "this is a test");

        cmdManager.showCommands();
        run();
    }
	
    int test2(Cmd::Args const& args);
    void run();
};
#endif