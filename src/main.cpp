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
#include "Utility.hpp"

//! The parse tree root of user input
CmdBase* cmdTreeRoot = NULL;
//! The command executor
Executor* cmdExecutor = NULL;

//! Handler for command 'exit'
void exitHandler(int /* status */)
{
    // clean up
    if (cmdTreeRoot) delete cmdTreeRoot;
    if (cmdExecutor) delete cmdExecutor;
}

int main(int argc, char* argv[])
{
    CmdParser parser;
    string input;
    int status = 127; // 127 denotes `command not found`.

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
            status = cmdTreeRoot->execute();
            delete cmdTreeRoot;
        }
    }

    delete cmdExecutor;
    return status;
}
