#ifndef __EXECUTOR__
#define __EXECUTOR__

class Executor
{
private:
    bool exitExecuted;

public:
    Executor() : exitExecuted(false) {}

    bool isExitExecuted() const { return exitExecuted; }

    int execute(std::string executable,
                std::vector<std::string> argList)
    {
        if (executable == "exit")
        {
            exitExecuted = true;
            return 0;
        }
        else
        {
            // TODO: Invoke system calls
            std::cout << executable;
            for (unsigned int i = 0; i < argList.size(); i++)
                std::cout << " " << argList[i];
            std::cout << endl;
            return 0;
        }
    }
};

#endif
