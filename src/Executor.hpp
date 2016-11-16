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
            struct stat filechecker;   //used for checking if files/directories exist,regular,and is directory
            int truetracker=1;   //used for checking wheter to return 0 or 1 and print False or True
            
            if( argList.size()>1 && strcmp(argList[1],"-f")==0)   //-f case
            {
                if(argList.size()>2)
                {
                    if(stat(argList[2],&filechecker)==0 && S_ISREG(filechecker.st_mode))   //if it exists and is a regular file truetracker set to 0
                    {
                        truetracker=0;
                    }
                    else   //else truetracker is set to 1
                    {
                        truetracker=1; 
                    }
                    
                }
            }
            
            else if(argList.size()>1 && strcmp(argList[1],"-d")==0)   //-d case
            {
                if(argList.size()>2)
                {
                    if(stat(argList[2],&filechecker)==0 && S_ISDIR(filechecker.st_mode))   //if it exists and is a regular directory truetracker set to 0
                    {
                        truetracker=0;
                    }
                    else   //else truetracker is set to 1
                    {
                        truetracker=1;
                    }
                }
            }
            
            else   //-e case (default case if no specification)
            {
                int ecase=0;   //used for determining where to start checking if file/directory exists
                if(argList.size() >1 && strcmp(argList[1],"-e")==0)   //case where -e is specified start checking at argList[2]
                {
                    ecase=2;
                }
                
                else   //case where -e is not specified start checking at argList[1]
                {
                    ecase=1;
                }
                
                if(argList.size()>=2)
                {
                    if(stat(argList[ecase],&filechecker)==0 && (S_ISREG(filechecker.st_mode) || S_ISDIR(filechecker.st_mode)))   //if the file/directory exists set truetracker to 0
                    {
                        truetracker=0;
                    }
                    else   //else truetracker set to 1
                    {
                        truetracker=1;
                    }
                    
                }
                
            }
            
            if(truetracker==0) //if truetracker was set to 0, then print (True) and return 0
            {
                cout<<"(True)"<<endl;
                return 0;
            }
            else   //else print (False) and return 1
            {
                cout<<"(False)"<<endl;
                return 1;
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