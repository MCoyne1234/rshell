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

    //! Whether the Executor has executed @c exit command.
    bool isExitExecuted() const { return exitExecuted; }

    void setExitHandler(ExitHandler handler) { this->exitHandler = handler; }

    /**
     * @brief Execute a given executable with argument list.
     *
     * Note that the first element of @c argList (that is, @c argList[0])
     * should have the same content as @c executable and the last element
     * should be NULL, which is required by @c execvp().
     *
     * @param executable The executable's path or filename.
     * @param argList The argument list.
     * @return The exit status of executing the @c executable.
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
                if (isNumber(argList[1]))
                    exitShell(atoi(argList[1]));
                else
                {
                    std::cerr << SHELL_NAME ": exit: ";
                    std::cerr << "numeric argument required" << std::endl;
                    exitShell(128); // 128 for invalid argument to exit
                }
            }
        }
        else if (executable == "cd")
        {
            char *chPath = argList[1];
            
            // No argument? Change to home dir.
            if (argList[1] == NULL)
            {
                chPath = getenv("HOME");
                if (chPath == NULL)
                {
                    // Note that getenv will not set errno.
                    std::cerr << SHELL_NAME ": cd: HOME not set" << std::endl;
                    return 1; // 1 for general error.
                }
            }
            else if (!strcmp(argList[1], "-"))
            {
                chPath = getenv("OLDPWD");
                if (chPath == NULL)
                {
                    std::cerr << SHELL_NAME ": cd: OLDPWD not set" << std::endl;
                    return 1; // 1 for general error.
                }
            }
            // Otherwise, chPath should be argList[1],
            // which we have set that before.
            
            // Current PWD would be OLDPWD.
            std::string oldPwd = getCurrentAbsolutePath();
            
            if (chdir(chPath) != 0)
                return printSysError(std::string("cd: ") + chPath);
            else
            {
                // Set PWD and OLDPWD.
                if (-1 == setenv("PWD", getCurrentAbsolutePath().c_str(), 1))
                    return printSysError(std::string("setenv(PWD)"));
                
                if (-1 == setenv("OLDPWD", oldPwd.c_str(), 1))
                    return printSysError(std::string("setenv(OLDPWD)"));
                
                return 0;
            }
        }
        else if (executable == "test" || executable == "[")
        {
            const int TestTrue = 0, TestFalse = 1;
            
            // Remove the last element (NULL).
            argList.pop_back();
            
            // For [, check if ] is missing first.
            if (executable == "[")
            {
                // We have ], so pop it.
                if (!strcmp(argList.back(), "]"))
                    argList.pop_back();
                else
                {
                    std::cerr << SHELL_NAME ": " << executable;
                    std::cerr << ": missing `]`" << std::endl;
                    return 1; // 1 for general errors
                }
            }
            
            // Now, we remove the first element (the executable).
            argList.erase(argList.begin());
            
            // Check if we still got arguments left?
            // Otherwise, return false, following the 0 arguments rule.
            if (argList.empty())
            {
                std::cout << "(False)" << endl;
                return TestFalse;
            }

            // The default flag is `-e`.
            char flag = 'e';
            // Check the first argument if it is a flag.
            if (argList[0][0] == '-')
            {
                if (strcmp(argList[0], "-e") == 0)
                    flag = 'e';
                else if (strcmp(argList[0], "-f") == 0)
                    flag = 'f';
                else if (strcmp(argList[0], "-d") == 0)
                    flag = 'd';
                else
                {
                    std::cerr << SHELL_NAME ": " << executable;
                    std::cerr << ": unknown flag " << argList[0] << std::endl;
                    return 1; // 1 for general errors
                }

                // Then we remove it.
                argList.erase(argList.begin());
            }
            
            // We should have an argument denoting path.
            if (argList.empty())
            {
                std::cerr << SHELL_NAME ": " << executable;
                std::cerr << ": missing argument" << std::endl;
                return 1; // 1 for general errors
            }
            
            // Invoke syscall.
            struct stat sb;
            int ret = stat(argList[0], &sb);
            // If stat failed, but errno is not `file or dir not exist`
            if (ret == -1 && errno != ENOENT)
                return printSysError("stat()");
            
            // Perform the test.
            bool testResult = false;
            if (ret == 0)
            {
                switch (flag)
                {
                    case 'e':
                        testResult = S_ISDIR(sb.st_mode) || S_ISREG(sb.st_mode);
                        break;
                        
                    case 'd':
                        testResult = S_ISDIR(sb.st_mode);
                        break;
                        
                    case 'f':
                        testResult = S_ISREG(sb.st_mode);
                        break;
                }
            }
            // else - `stat` returns -1, denotes nonexistence.
            // Since testResult if false default, we don't need extra actions.
            
            // Return 0 on true, 1 on false.
            if (testResult)
            {
                std::cout << "(True)" << std::endl;
                return TestTrue;
            }
            else
            {
                std::cout << "(False)" << std::endl;
                return TestFalse;
            }
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
     *        (encapsulates @c perror).
     * @param cmd The failed command or syscall.
     * @return The error number (@c errno).
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

    /**
     * @brief Test if a give string is a number.
     * @param str The string to be tested.
     * @return Whether the string is a number.
     */
    bool isNumber(const char* str)
    {
        // Negative?
        if (*str == '-') ++str;
        // Empty string?
        if (*str == '\0') return false;

        // Scan until end
        while (*str != '\0')
            if (!isdigit(*str++))
                return false;

        return true;
    }
};

#endif
