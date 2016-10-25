#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
using namespace std;

#include <unistd.h>

#include "CmdBase.hpp"
#include "Executor.hpp"
#include "CmdUnary.hpp"
#include "CmdSequence.hpp"
#include "CmdBinary.hpp"
#include "CmdParser.hpp"

string getCurrentUserName()
{
    char* userName = getlogin();
    return userName ? string(userName) : "??name??";
}

string getCurrentHostName()
{
    char hostName[256];
    return gethostname(hostName, 255) < 0 ? "??host??" : string(hostName);
}

string getLoginInfo()
{
    return getCurrentUserName() + "@" + getCurrentHostName();
}

int main(int argc, char* argv[])
{
    string input, loginInfo = getLoginInfo();
    CmdBase* cmdTreeRoot = NULL;
    Executor* executor = new Executor();
    CmdParser parser;

    while (!cin.eof())
    {
        // If stdin was not redirected, print the prompt.
#ifndef DEBUG
        if (isatty(STDIN_FILENO))
#endif
            cout << loginInfo << " $ ";

        getline(cin, input); // Read the whole line.

        cmdTreeRoot = parser.parse(input, executor);
        if (cmdTreeRoot)
        {
            cmdTreeRoot->execute();
            delete cmdTreeRoot;
        }

        if (executor->isExitExecuted())
            break;
    }

    delete executor;
    return 0;
}
