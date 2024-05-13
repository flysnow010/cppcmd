#include "cmdtest.h"
#include <iostream>

void CppCmdSuite::run()
{
    cmdManager.run("RU->");
}

int test1(int argc, char* argv[])
{
    for(int i = 0; i < argc; i++)
        std::cout << argv[i] << ",";
    std::cout << std::endl;
    return argc;
}

int CppCmdSuite::test2(Cmd::Args const& args)
{
    for(auto arg = args.begin(); arg != args.end(); ++arg)
        std::cout << *arg << ",";
    std::cout << std::endl;
    return args.size();
}