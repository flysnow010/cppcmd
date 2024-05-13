#ifndef _CMD_HELPER_H_
#define _CMD_HELPER_H_
#include "cppcmd.h"
#include "cmdio.h"

#include <memory>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace Cmd
{
struct Command
{
    Command(Handler _handler = Handler(),
        const char *_cmd = nullptr,
        const char *_help = nullptr,
        const char *_group = nullptr
        )
    : cpp_handler(_handler)
    , name(_cmd ? _cmd : std::string())
    , help(_help ? _help : std::string())
    , group(_group ? _group : std::string())
    {}

    Command(handler _handler = handler(),
        const char *_cmd = nullptr,
        const char *_help = nullptr,
        const char *_group = nullptr
        )
    : c_hanlder(_handler)
    , name(_cmd ? _cmd : std::string())
    , help(_help ? _help : std::string())
    , group(_group ? _group : std::string())
    {}

    typedef std::shared_ptr<Command> Ptr;

    bool empty() const 
    {
        return name.empty() || 
            (cpp_handler == nullptr 
                && c_hanlder == nullptr); 
    }

    size_t cmd_len() const { return name.size(); }

    Handler cpp_handler;
    handler c_hanlder;
    std::string name;
	std::string help;
	std::string group;
};

class CommandHelper
{
    std::list<Command::Ptr> commands_;
    volatile bool signal_ =  false;
    int retValue_ = 0;
public:
    bool signal() const { return signal_; }
    void doSingal() { signal_ = true; }
    inline int returnValue() const { return retValue_; }

    inline bool add(Command::Ptr const& command)
    {
        if(command->empty() || find(command->name))
            return false;
        
        commands_.push_back(command);
        return true;
    }

    bool remove(std::string const& name)
    {
        if(!name.empty())
        {
            auto c = std::find_if(commands_.begin(), commands_.end(), 
                [&](Command::Ptr const& c) { return name == c->name; });
            if(c != commands_.end())
            {
                commands_.erase(c);
                return true;
            }
        }
        return false;
    }

    inline size_t size() const { return commands_.size(); }

    inline size_t cmd_maxlen() const
    {
        auto compare = [](Command::Ptr const& l, Command::Ptr const& r) {
             return l->cmd_len() < r->cmd_len(); };
        auto command = std::max_element(commands_.begin(), commands_.end(), compare);
        if(command == commands_.end())
            return 0;
        return (*command)->cmd_len();
    }

    Command::Ptr find(std::string const& name) const
    {
        if(!name.empty())
        {
            auto command = std::find_if(commands_.begin(), commands_.end(), 
                [&](Command::Ptr const& c) { return name == c->name; });
            if(command != commands_.end())
                return *command;
        }
        return Command::Ptr();
    }

    void showHelp(std::string const& help, size_t maxlen) const
    {
        auto pos = help.find(NL);
        if(pos == std::string::npos)
            std::cout << help;
        else
        {
            decltype(pos) start = 0;
            do
            {
                std::cout << help.substr(start, pos - start) << std::endl;
                std::cout << std::left << std::setw(maxlen) << "";

                start = pos + 1;
                pos = help.find(NL, start);
                if(pos == std::string::npos)
                {
                    std::cout << help.substr(start, help.size() - start);
                    break;
                }
            } while(true);
        }
        std::cout << std::endl;
    }

    bool show(std::string const& name, bool is_print_error = true) const
    {
        auto command = find(name);
        if(command)
        {
            std::cout << std::left << std::setw(command->cmd_len()) 
                << command->name << " : ";
            showHelp(command->help, command->cmd_len() + 3);
            return true;
        }
        if(is_print_error)
            std::cerr << "cannot find cmd: " << name << std::endl;
        return false;
    }

    int showGroup(std::string const& group) const
    {
        int count = 0;
        std::string g;
        size_t maxlen = cmd_maxlen();

        for(auto command = commands_.begin(); 
            command != commands_.end(); ++command)
        {
            if(g.empty())
            {
                g =  (*command)->group;
                if(group.empty())
                    std::cout << "[" << g << "]" << std::endl;
            }
            else if(g != (*command)->group)
            {
                g =  (*command)->group;
                if(group.empty())
                    std::cout << std::endl 
                        << "[" << g << "]" << std::endl;
            }

            if(group.empty() || group == (*command)->group)
            {
                std::cout << std::left << std::setw(maxlen) << (*command)->name << " : ";
                showHelp((*command)->help, maxlen + 3);
                count++;
            }
        }
        std::cout << std::endl;
        if(group.empty())
            std::cout << "show " << count << " commands" << std::endl << std::endl;
        return count;
    }

    void addDefault()
    {
        add(std::make_shared<Command>([=](Args const& args) {
            std::cout << "ret = " << retValue_ << std::endl;
            return retValue_;
        }, "ret", "get return value of last command", "sys"));

        add(std::make_shared<Command>([=](Args const& args) {
            for(size_t i = 0; i < args.size(); i++) {
                std::cout << args[i];
                if(i != args.size() - 1)
                    std::cout << " ";
                else
                    std::cout << std::endl;   
	        }
            return retValue_;
        }, "print", "print text", "sys"));

        add(std::make_shared<Command>([=](Args const& args) {
            if(args.size() < 2)
                return retValue_;

            int value = std::stol(args[1]);
            if(value == retValue_)
                std::cout << "Test of " << args[0] << " is passed(ok)!" << std::endl;
            else
                std::cout << "Test of " << args[0] << " is not passed!" << std::endl;
            return retValue_;
        }, "test", "test return value of last command." S "eg. test funname retvalue", "sys"));

        add(std::make_shared<Command>([=](Args const& args) {
            std::cout << "Press enter key..." << std::endl;
            CmdIO().readAnyKey();
            return retValue_;
        }, 
        "pause", "pause util press enter key", "sys"));

        add(std::make_shared<Command>([&](Args const& args) {
            std::string group;
            std::cout << "groups:" << std::endl;
            for(auto command = commands_.begin(); 
                command != commands_.end(); ++command)
            {
                if(group.empty() || group != (*command)->group)
                {
                    group =  (*command)->group;
                    std::cout << " " << group << std::endl;
                }
            }
            return retValue_;
        }, 
        "group", "show groups", "sys"));

        add(std::make_shared<Command>([=](Args const& args) {
            doSingal();
            return 0;
        }, 
        "exit", "exit the program", "sys"));

        add(std::make_shared<Command>([=](Args const& args) {
            std::string group;

            if(args.size() == 2 && args[1] == "group")
                group = args[0];
            else if(args.size() == 1) {
                if(show(args[0], false))
                    return 0;
		        group = args[0];
            }
            showGroup(group);
            return retValue_;
        }, 
        "help", "show this page." S "eg. help command/group", "sys"));
    }

    bool parse(std::string const& line)
    {
        std::string name;
        std::istringstream is(line);
        
        is >> name;

        Command::Ptr command = find(name);
        if(!command)
        {
            std::cerr << "cannot find command: " << name << std::endl;
            return false;
        }
        else
        {
            std::vector<std::string> args = get_args(is);
            if(command->cpp_handler)
                retValue_ = command->cpp_handler(args);
            else
            {
                std::vector<char*> argv(args.size(), 0);
                for(size_t i = 0; i < args.size(); i++)
                    argv[i] = (char *)args[i].c_str();
                
                retValue_ = command->c_hanlder(argv.size(), argv.size() ? &argv[0] : 0);
            }
            if(retValue_ < 0)
                show(name);
        }
        return true;
    }
private:
    Args get_args(std::istringstream &is) 
    {
        std::string arg;
        Args args;
        const char MARK1 = '"';
        const char MARK2 = '\'';
        const char SPACE = ' ';
        char mark = 0;

        while(true)
        {
            char ch;
            if(is.get(ch).eof())
            {
                if(!arg.empty())
                {
                    if(!mark)
                        args.push_back(std::move(arg));
                    else
                    {
                        Args subArgs = get_args(arg);
                        args.insert(args.end(), subArgs.begin(), subArgs.end());
                    }
                }
                break;
            }

            if(ch != SPACE)
                arg.push_back(ch);

            if(ch == MARK1 || ch == MARK2)
            {
                if(!mark)
                    mark = ch;
                else
                {
                    if(mark == ch)
                        args.push_back(arg.substr(1, arg.size() - 2));
                    else
                    {
                        Args subArgs = get_args(arg);
                        args.insert(args.end(), subArgs.begin(), subArgs.end());
                    }
                    arg = std::string();
                    mark = 0;
                }
            }
            else if(ch == SPACE)
            {
                if(mark)
                    arg.push_back(ch);
                else if(!arg.empty())
                    args.push_back(std::move(arg));
            }
        }
        return args;
    }

    Args get_args(std::string const& line)
    {
        Args args;
        std::istringstream is(line);
        while(!is.eof())
        {
            std::string arg;
            is >> arg;
            args.push_back(std::move(arg));
        }
        return args;
    }
};
}
#endif