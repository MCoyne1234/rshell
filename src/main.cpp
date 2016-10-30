#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
using namespace std;

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define SHELL_NAME "rshell"

#include "CmdBase.hpp"
#include "Executor.hpp"
#include "CmdUnary.hpp"
#include "CmdSequence.hpp"
#include "CmdBinary.hpp"
#include "CmdParser.hpp"

string getCurrentUserName()
{
    char* userName = getlogin();
    if (userName == NULL)
        perror(SHELL_NAME);

    return userName ? string(userName) : "??name??";
}

string getCurrentHostName()
{
    char hostName[256];

    int ret = gethostname(hostName, 256);
    if (ret < 0)
        perror(SHELL_NAME);

    return ret >= 0 ? string(hostName) : "??host??";
}

string getCurrentAbsolutePath()
{
    char currentDir[1024];

    char* ret = getcwd(currentDir, 1024);
    if (ret == NULL)
        perror(SHELL_NAME);

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

string getPromptInfo()
{
    return getCurrentUserName() + "@" + getCurrentHostName() + ": " +
           getCurrentRelativePath() + " $ ";
}

int main(int argc, char* argv[])
{
    string input;
    CmdBase* cmdTreeRoot = NULL;
    Executor* executor = new Executor();
    CmdParser parser;

    while (!cin.eof())
    {
        cout << getPromptInfo();
        getline(cin, input); // Read the whole line.

        cmdTreeRoot = parser.parse(input, executor);
        cmdTreeRoot->execute();
        delete cmdTreeRoot;

        if (executor->isExitExecuted())
            break;
    }

    delete executor;
    return 0;
}
