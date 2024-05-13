# 1 概述
&emsp;&emsp;在嵌入式开发中，经常会利用命令行交互系统测试功能，本文描述使用C++语言开发的一个命令行交互系统，支持命令注册(C或C++格式)，命令分组，执行命令，显示命令提示。
# 3 实现

## 3.1 CommandManager定义
```cpp
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

    bool registerCommand(const char* cmd, Handler handler, const char* help);
    bool registerCommand(const char* cmd, handler handler, const char* help);
    bool registerCommand(const char* cmd, 
                         const char* group, Handler handler, const char* help);
    bool registerCommand(const char* cmd,
                         const char* group, handler handler, const char* help);
    
    bool unregisterCommand(const char* cmd);

    bool showCommand(const char* cmd) const;
    int  showCommands() const;
    int  showCommands(const char* group) const;

    bool execCommand(const char* cmdString) const;
    
    int  retrunValue() const;

    int run(const char* prompt);
};
```
接口说明：
- commandSize 注册的命令数
- registerCommand 注册命令
- unregisterCommand 注销命令
- showCommand 显示单条命令信息
- showCommands 按组显示命令
- execCommand 执行命令
- returnValue 获取命令返回值
- run 运行命令行系统，通过默认命令exit退出
## 3.2 CommandManager实现
```cpp
CommandManager::CommandManager()
: helpr_(new CommandHelper())
{
}

CommandManager::~CommandManager() { delete helpr_; }

size_t CommandManager::commandSize() const { return helpr_->size(); }

bool CommandManager::registerCommand(const char* cmd, 
    Handler handler, const char* help)
{
    return helpr_->add(std::make_shared<Command>(handler, cmd, help));
}

bool CommandManager::registerCommand(const char* cmd, 
    handler handler, const char* help)
{
    return helpr_->add(std::make_shared<Command>(handler, cmd, help));
}

bool CommandManager::registerCommand(const char* cmd, const char* group, 
    Handler handler, const char* help)
{
    return helpr_->add(std::make_shared<Command>(handler, cmd, help, group));
}

bool CommandManager::registerCommand(const char* cmd, const char* group, 
    handler handler, const char* help)
{
    return helpr_->add(std::make_shared<Command>(handler, cmd, help, group));
}

bool CommandManager::unregisterCommand(const char* cmd)
{
    return helpr_->remove(cmd ? cmd : std::string());
}

bool CommandManager::showCommand(const char* cmd) const
{
    return helpr_->show(cmd ? cmd : std::string());
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
```
函数说明:
- run 运行命令行系统
  - 调用helpr_的addDefault函数注册默认命令
  - 调用io函数的readLine读取命令行
  - 调用helpr_的parse解析并执行命令
  - 重复上面两步，直到调用exit命令退出
- 其它函数直接调用CommandHelper对应函数。
## 3.3 Command
```cpp
struct Command
{
    Command(Handler _handler = Handler(),
        const char *_cmd = nullptr,
        const char *_help = nullptr,
        const char *_group = nullptr
        )
    : cpp_handler(_handler)
    , cmd(_cmd ? _cmd : std::string())
    , help(_help ? _help : std::string())
    , group(_group ? _group : std::string())
    {}

    Command(handler _handler = handler(),
        const char *_cmd = nullptr,
        const char *_help = nullptr,
        const char *_group = nullptr
        )
    : c_hanlder(_handler)
    , cmd(_cmd ? _cmd : std::string())
    , help(_help ? _help : std::string())
    , group(_group ? _group : std::string())
    {}

    typedef std::shared_ptr<Command> Ptr;

    bool empty() const 
    {
        return cmd.empty() || 
            (cpp_handler == nullptr 
                && c_hanlder == nullptr); 
    }
    
    size_t cmd_len() const { return cmd.size(); }

    Handler cpp_handler;
    handler c_hanlder;
    std::string name;
	std::string help;
	std::string group;
};
```
成员列表:
- cpp_handler C++命令处理器
- c_hanlder   C命令处理器
- name 命令名称
- help 命令帮助
- group 命令组

接口列表：
- empty 命令是否为空
  
## 3.4 CommandHelper
```cpp
class CommandHelper
{
    std::list<Command::Ptr> commands_;
    volatile bool signal_ =  false;
    int retValue_ = 0xFFFFFFFF;
public:
    bool signal() const { return signal_; }
    void doSingal() { signal_ = true; }
    inline int returnValue() const { return retValue_; }

    inline bool add(Command::Ptr const& command)
    {
        if(command->empty() || find(command->cmd))
            return false;
        
        commands_.push_back(command);
        return true;
    }

    bool remove(std::string const& cmd)
    {
        if(!cmd.empty())
        {
            auto c = std::find_if(commands_.begin(), commands_.end(), 
                [&](Command::Ptr const& c) { return cmd == c->cmd; });
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

    Command::Ptr find(std::string const& cmd) const
    {
        if(!cmd.empty())
        {
            auto command = std::find_if(commands_.begin(), commands_.end(), 
                [&](Command::Ptr const& c) { return cmd == c->cmd; });
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

    bool show(std::string const& cmd, bool is_print_error = true) const
    {
        auto command = find(cmd);
        if(command)
        {
            std::cout << std::left << std::setw(command->cmd_len()) 
                << command->cmd << " : ";
            showHelp(command->help, command->cmd_len() + 3);
            return true;
        }
        if(is_print_error)
            std::cerr << "cannot find cmd: " << cmd << std::endl;
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
                g =  (*command)->group;
            else if(g != (*command)->group)
            {
                g =  (*command)->group;
                std::cout << std::endl;
            }

            if(group.empty() || group == (*command)->group)
            {
                std::cout << std::left << std::setw(maxlen) << (*command)->cmd << " : ";
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
        std::string cmd;
        std::istringstream is(line);
        
        is >> cmd;

        Command::Ptr command = find(cmd);
        if(!command)
        {
            std::cerr << "cannot find command: " << cmd << std::endl;
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
        }
        return true;
    }
```
接口列表:
- signal 是否是退出信号，默认是false
- doSingal 设置退出信号
- add 增加命令
- remove 删除命令
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

## 3.5 CmdIO定义
```cpp
class CmdIO
{
public:
    std::string readLine(const char* prompt);
    void readAnyKey();
};
```
接口列表:
- readLine 从命令上读取一行
- readAnyKey 读取任意按键

## 3.6 CmdIO实现
```cpp
std::string CmdIO::readLine(const char* prompt)
{
	std::string line;
	std::cout << prompt;
	std::getline(std::cin, line);
	return line;
}

void CmdIO::readAnyKey() { fgetc(stdin); }
```
说明:
- readLine 调用getline获得命令行
- readAnyKey 调用fgetc读取一个按键