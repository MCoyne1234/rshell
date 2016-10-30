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
        // execvp() requires the executable as the first element of arguments,
        // and NULL as the last element in argument list, so we need to
        // construct a new argument list for the executor.
        std::vector<char*> args;
        args.push_back((char*) executable.c_str());
        for (unsigned int i = 0; i < argList.size(); i++)
            args.push_back((char*) argList[i].c_str());
        args.push_back(NULL);

        return executor->execute(executable, args);
    }

    std::string toString()
    {
        std::string ret = executable;
        for (unsigned int i = 0; i < argList.size(); i++)
            ret += " " + argList[i];
        return ret;
    }
};

#endif
