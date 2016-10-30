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
                std::vector<char*> argList)
    {
        if (executable == "exit")
        {
            exitExecuted = true;

            if (argList[1] == NULL)
                return 0;
            else // Exit with status
            {
                // Assume user input is numeric
                bool numeric = true;

                // Check if it is true
                for (size_t i = 0; i < strlen(argList[1]); i++)
                {
                    if (!isdigit(argList[1][i]))
                    {
                        numeric = false;
                        break;
                    }
                }

                if (numeric)
                    return atoi(argList[1]);
                else
                {
                    std::cerr << SHELL_NAME ": exit: ";
                    std::cerr << "numeric argument required" << std::endl;
                    return 1; // 1 for general errors
                }
            }
        }
        else if (executable == "cd")
        {
            if (argList[1] == NULL)
            {
                std::cerr << SHELL_NAME ": cd: missing argument" << std::endl;
                return 1; // 1 for general errors
            }
            else
            {
                if (chdir(argList[1]) == 0)
                    return 0;
                else
                    return printSysError(std::string("cd: ") + argList[1]);
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
                    wpid = waitpid(pid, &status, 0);
                    if (wpid < 0) // waitpid() failed
                        return printSysError("waitpid()");
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));

                // Return child process's exit status
                return WEXITSTATUS(status);
            }
        }
    }

private:
    int printSysError(std::string cmd)
    {
        std::string prefix = std::string(SHELL_NAME ": ") + cmd;
        perror(prefix.c_str());
        return errno;
    }
};

#endif
