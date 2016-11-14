#ifndef __EXECUTOR__
#define __EXECUTOR__

//! The class for executing command.
class Executor
{
public:
    //! The handler type for handling 'exit' command.
    typedef void(*ExitHandler)(int);

private:
    bool exitExecuted;
    ExitHandler exitHandler;

public:
    Executor() : exitExecuted(false), exitHandler(NULL) {}
    /**
     * @brief Construct a Exectuor with a ExitHandler.
     * @param handler The exit handler.
     */
    Executor(ExitHandler handler) :
            exitExecuted(false), exitHandler(handler) {}

    //! Whether the Executor has executed \c exit command.
    bool isExitExecuted() const { return exitExecuted; }

    void setExitHandler(ExitHandler handler) { this->exitHandler = handler; }

    /**
     * @brief Execute a given executable with argument list.
     *
     * Note that the first element of \c argList (that is, \c argList[0])
     * should have the same content as \c executable and the last element
     * should be NULL, which is required by \c execvp().
     *
     * @param executable The executable's path or filename.
     * @param argList The argument list.
     * @return The exit status of executing the \c executable.
     */
    int execute(std::string executable,
                std::vector<char*> argList)
    {
        if (executable == "exit")
        {
            exitExecuted = true;

            if (argList[1] == NULL)
                exitShell(0);
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
                    exitShell(atoi(argList[1]));
                else
                {
                    std::cerr << SHELL_NAME ": exit: ";
                    std::cerr << "numeric argument required" << std::endl;
                    exitShell(1); // 1 for general errors
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
        else if (executable == "test" || executable == "[")
        {
            // TODO: Implement `test` command.
            // Copied from the man page of `test`:
            //     The test utility evaluates the expression and, if it
            //     evaluates to true, returns 0 (true); otherwise it
            //     returns 1 (false).  If there is no expression,
            //     test also returns 1 (false).
            return 1;
        }
        else
        {
            pid_t pid, wpid;
            int status;

            pid = fork();
            if (pid < 0) // fork() failed
                exitShell(printSysError("fork()"));
            else if (pid == 0) // Child process
            {
                // &argList[0] denotes the vector's internal array.
                if (execvp(executable.c_str(), &argList[0]) < 0)
                {
                    if (errno == ENOENT) // No such file or directory
                        exitShell(printSysError(executable.c_str()));
                    else
                        exitShell(printSysError("execvp()"));
                }

                // Actually, we shall not reach here. Because execvp()
                // should take control of our child process.
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

        // Actually, we shall not reach here.
        return 1; // 1 for general errors
    }

private:
    /**
     * @brief Helper function for printing failure of syscalls or commands
     *        (encapsulates \c perror).
     * @param cmd The failed command or syscall.
     * @return The error number (\c errno).
     */
    int printSysError(std::string cmd)
    {
        std::string prefix = std::string(SHELL_NAME ": ") + cmd;
        perror(prefix.c_str());
        return errno;
    }

    /**
     * @brief Helper function for exit, would call the exitHandler.
     * @param status The exit status.
     */
    void exitShell(int status)
    {
        if (exitHandler != NULL)
            exitHandler(status);

        exit(status);
    }
};

#endif
