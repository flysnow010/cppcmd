#include "cppcmd.h"
#include "cmdhelper.h"
#include "cmdio.h"

#include <iostream>

namespace Cmd
{
CommandManager::CommandManager()
: helpr_(new CommandHelper())
{
}

CommandManager::~CommandManager()
{
    delete helpr_;
}

size_t CommandManager::commandSize() const
{
    return helpr_->size();
}

bool CommandManager::registerCommand(const char* name, 
    Handler handler, const char* help)
{
    return helpr_->add(std::make_shared<Command>(handler, name, help));
}

bool CommandManager::registerCommand(const char* name, 
    handler handler, const char* help)
{
    return helpr_->add(std::make_shared<Command>(handler, name, help));
}

bool CommandManager::registerCommand(const char* name, const char* group, 
    Handler handler, const char* help)
{
    return helpr_->add(std::make_shared<Command>(handler, name, help, group));
}

bool CommandManager::registerCommand(const char* name, const char* group, 
    handler handler, const char* help)
{
    return helpr_->add(std::make_shared<Command>(handler, name, help, group));
}

bool CommandManager::unregisterCommand(const char* name)
{
    return helpr_->remove(name ? name : std::string());
}

bool CommandManager::showCommand(const char* name) const
{
    return helpr_->show(name ? name : std::string());
}

int CommandManager::showCommands() const
{
    return helpr_->showGroup(std::string());
}

int CommandManager::showCommands(const char* group) const
{
    return helpr_->showGroup(group ? group : std::string());
}

bool CommandManager::execCommand(const char* line) const
{
    return helpr_->parse(line ? line : std::string());
}

int CommandManager::retrunValue() const
{
    return helpr_->returnValue();
}

int CommandManager::run(const char* prompt)
{
    CmdIO io;
    helpr_->addDefault();
    while(!helpr_->signal())
    {
        std::string line = io.readLine(prompt);
        if(line.empty())
            continue;
        
        helpr_->parse(line);
    }
    return 0;
}
}