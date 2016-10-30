#ifndef __EXECUTOR__
#define __EXECUTOR__

class Executor
{
private:
    bool exitExecuted;

private:
    int printSysError(std::string cmd)
    {
        std::string suffix = string(SHELL_NAME ": ") + cmd;
        perror(suffix.c_str());
        return errno;
    }

public:
    Executor() : exitExecuted(false) {}

    bool isExitExecuted() const { return exitExecuted; }

    int execute(std::string executable,
                std::vector<char*> argList)
    {
        if (executable == "exit")
        {
            exitExecuted = true;
            return 0;
        }
        else if (executable == "cd")
        {
            if (argList.size() < 2 || argList[1] == NULL)
            {
                cerr << SHELL_NAME ": cd: missing argument." << endl;
                return 1;
            }
            else
            {
                return chdir(argList[1]) != 0 ? printSysError("chdir()") : 0;
            }
        }
        else
        {
            pid_t pid, wpid;
            int status;

            pid = fork();
            if (pid < 0) // fork() failed
                return printSysError("fork()");
            else if (pid == 0) // Child process
            {
                // &argList[0] denotes the vector's internal array.
                if (execvp(executable.c_str(), &argList[0]) < 0)
                {
                    if (errno == ENOENT) // No such file or directory
                        return printSysError(executable.c_str());
                    else
                        return printSysError("execvp()");
                }

                // Actually, we shall not reach here. Because execvp() should
                // take control of our child process.
                exit(1);
            }
            else // Parent process
            {
                // Loop until child process exited or signaled.
                do
                {
                    wpid = waitpid(pid, &status, WUNTRACED);
                    if (wpid < 0) // waitpid() failed
                        return printSysError("waitpid()");
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));

                // Return child process's exit status
                return status;
            }
        }
    }
    
};

#endif
