#ifndef _CMD_IO_H_
#define _CMD_IO_H_
#include <string>

namespace Cmd
{
class CmdIO
{
public:
    std::string readLine(const char* prompt);
    void readAnyKey();
};
}
#endif