#include <iostream>
#include <string>
#include <queue>
#include <deque>
#include <vector>
#include <algorithm>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define SHELL_NAME "rshell"

#include "CmdBase.hpp"
#include "Executor.hpp"
#include "CmdUnary.hpp"
#include "CmdSequence.hpp"
#include "CmdBinary.hpp"
#include "CmdParser.hpp"

//! The parse tree root of user input
CmdBase* cmdTreeRoot = NULL;
//! The command executor
Executor* cmdExecutor = NULL;
//! The exit status of rshell
int exitStatus = 127; // 127 denotes `command not found`.

string getCurrentUserName()
{
    char* userName = getlogin();
    if (userName == NULL)
        perror(SHELL_NAME ": getlogin()");

    return userName ? string(userName) : "??name??";
}

string getCurrentHostName()
{
    char hostName[256];

    int ret = gethostname(hostName, 256);
    if (ret < 0)
        perror(SHELL_NAME ": gethostname()");

    return ret >= 0 ? string(hostName) : "??host??";
}

string getCurrentAbsolutePath()
{
    char currentDir[1024];

    char* ret = getcwd(currentDir, 1024);
    if (ret == NULL)
        perror(SHELL_NAME ": getcwd()");

    return ret != NULL ? string(currentDir) : "??dir??";
}

string getCurrentRelativePath()
{
    string absPath = getCurrentAbsolutePath();
    string::size_type lastSlashPos = absPath.find_last_of("/");

    // When getcwd() failed, there is no `/` in the absolute path.
    if (lastSlashPos == string::npos)
        return absPath;
    else
    {
        // This occurred only when absolute path is `/`.
        if (lastSlashPos == absPath.length() - 1)
            return absPath;
        else
            return absPath.substr(lastSlashPos + 1);
    }
}

//! Return the prompt with current logined user name, host name and
//! working directory, such as `example\@hammer.cs.ucr.edu: var $`.
string getPromptInfo()
{
    return getCurrentUserName() + "@" + getCurrentHostName() + ": " +
           getCurrentRelativePath() + " $ ";
}

//! Handler for command 'exit'
void exitHandler(int status)
{
    exitStatus = status;

    // clean up
    if (cmdTreeRoot) delete cmdTreeRoot;
    if (cmdExecutor) delete cmdExecutor;
}

int main(int argc, char* argv[])
{
    CmdParser parser;
    string input;

    // Set up executor with our handler
    cmdExecutor = new Executor(exitHandler);
    while (!cin.eof())
    {
        // If stdin was not redirected, print the prompt.
        if (isatty(STDIN_FILENO))
            cout << getPromptInfo();

        getline(cin, input); // Read the whole line.

        cmdTreeRoot = parser.parse(input, cmdExecutor);
        if (cmdTreeRoot != NULL)
        {
            exitStatus = cmdTreeRoot->execute();
            delete cmdTreeRoot;
        }
    }

    delete cmdExecutor;
    return exitStatus;
}
