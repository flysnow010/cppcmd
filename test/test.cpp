#include "cmdtest.h"

int main(int argc, char *argv[])
{
    try
    {
        Test::Suite ts;
        ts.add(std::unique_ptr<Test::Suite>(new CppCmdSuite()));
        if(0)
        {
            Test::HtmlOutput output;
            ts.run(output);
            output.generate(std::cout, true, "cpptest");
        }
        else
        {
            Test::TextOutput output(Test::TextOutput::Verbose);
            ts.run(output);
        } 
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}