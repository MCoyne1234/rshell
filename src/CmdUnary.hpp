#ifndef __CMD_UNARY__
#define __CMD_UNARY__

class CmdUnary : public CmdBase
{
private:
    std::string executable;
    std::vector<std::string> argList;
    Executor* executor;

public:
    CmdUnary(std::string executable,
             std::vector<std::string> argList)
            : executable(executable), argList(argList)
    {}

    CmdUnary(std::string executable,
             std::vector<std::string> argList,
             Executor* executor)
            : executable(executable), argList(argList), executor(executor)
    {}

    Executor* getExecutor() const { return executor; }
    void setExecutor(Executor* executor) { this->executor = executor; }

    int execute() 
    {
        return executor->execute(executable, argList);
    }
};

#endif
