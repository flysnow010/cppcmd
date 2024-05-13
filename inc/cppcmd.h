#ifndef _CPP_CMD_H_
#define _CPP_CMD_H_
#include <functional>
#include <vector>
#include <string>
namespace Cmd
{
typedef std::vector<std::string> Args;
typedef std::function<int(int argc, char *argv[])> handler;
typedef std::function<int(Args const& args)> Handler;

#define PTR(Obj) &(Obj)
#define HANDLER(MemberFun, Ptr) \
    std::bind(&MemberFun, Ptr, std::placeholders::_1)

#define NL  "\n"
#define S    NL" "
#define SS   NL"  "
#define SSS  NL"   "
#define SSSS NL"    "

class CommandHelper;
class CommandManager
{
    CommandHelper* helpr_;
    CommandManager(CommandManager const&);
    void operator==(CommandManager const&);
public:
    CommandManager();
    ~CommandManager();

    size_t commandSize() const;

    bool registerCommand(const char* name, Handler handler, const char* help);
    bool registerCommand(const char* name, handler handler, const char* help);
    bool registerCommand(const char* name, 
                         const char* group, Handler handler, const char* help);
    bool registerCommand(const char* name,
                         const char* group, handler handler, const char* help);
    
    bool unregisterCommand(const char* name);

    bool showCommand(const char* name) const;
    int  showCommands() const;
    int  showCommands(const char* group) const;

    bool execCommand(const char* cmdString) const;

    int  retrunValue() const;

    int run(const char* prompt);
};
}
#endif