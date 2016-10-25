#ifndef __CMD_UNARY__
#define __CMD_UNARY__

class CmdUnary : public CmdBase
{
private:
    std::string executable;
    std::vector<std::string> argList;
    Executor* executor;

public:
    CmdUnary(std::vector<std::string> rawCmd,
             Executor* executor = NULL)
            : executor(executor)
    {
        executable = rawCmd.at(0);
        argList = std::vector<string>(rawCmd.begin() + 1, rawCmd.end());
    }

    CmdUnary(std::string executable,
             std::vector<std::string> argList,
             Executor* executor = NULL)
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
