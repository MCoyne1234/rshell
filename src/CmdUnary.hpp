#ifndef __CMD_UNARY__
#define __CMD_UNARY__

//! The class for single (unary) command.
class CmdUnary : public CmdBase
{
    typedef std::vector<std::string> ArgVector;

private:
    ArgVector argList;
    Executor* executor;

public:
    CmdUnary() : executor(NULL) {}
    
    /**
     * @brief Construct a single command by given argument list
     *        and Executor.
     * @param argList The argument list. 
     *                The first element should be the executable.
     * @param executor The Executor used in @c execute().
     */
    CmdUnary(std::vector<std::string> argList,
             Executor* executor = NULL)
            : argList(argList), executor(executor)
    {}

    ArgVector getArgList() const { return this->argList; }
    void setArgList(ArgVector argList) { this->argList = argList; }
    
    bool isArgListEmpty() const { return this->argList.empty(); }
    void pushArg(std::string arg) { this->argList.push_back(arg); }

    Executor* getExecutor() const { return executor; }
    void setExecutor(Executor* executor) { this->executor = executor; }

    int execute() 
    {
        // We need at least one argument to run the command.
        if (argList.empty())
            return 1; // 1 for general errors
        
        // execvp() requires the executable as the first element of arguments,
        // and NULL as the last element in argument list, so we need to
        // construct a new argument list for the executor.
        std::vector<char*> args;
        for (unsigned int i = 0; i < argList.size(); i++)
            args.push_back((char*) argList[i].c_str());
        args.push_back(NULL);

        return executor->execute(argList[0], args);
    }
};

#endif
