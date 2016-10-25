#ifndef __CMD_PARSER__
#define __CMD_PARSER__

class CmdParser
{
public:
    /**
     * @brief Parse user input and return the root of the parse tree.
     *
     * The tree hierarchy shall be like the following form:
     *
     * - CmdSequence
     *   - CmdUnary
     *   - CmdBinary
     *     - CmdUnary
     *     - CmdUnary
     *
     * @param input The user input.
     * @param executor The executor used in tree leaf CmdUnary.
     * @return The root of the parse tree.
     */
    CmdBase* parse(std::string input,
                   Executor* executor = NULL)
    {
        // The return value is a CmdSequence, because the input could always
        // be parsed into a sequence, even though only one command.
        CmdSequence* cmdSeq = new CmdSequence();

        // Firstly, use # as the delimiter, to retrieve the statement before #.
        std::vector<std::string> stmt = tokenize(input, "#");

        // Empty command?
        if (stmt.empty()) return NULL;

        // Secondly, use semi-colon as the delimiter, to generate a sequence.
        std::vector<std::string> seq = tokenize(stmt[0], ";");

        for (unsigned int i = 0 ; i < seq.size(); i++)
        {
            // Thirdly, we use two stacks to parse binary commands.
            std::stack<std::string> symStack;
            std::stack<CmdBase*> cmdStack;
            std::string command;
            std::vector<std::string> unary;

            // Iterate all characters, push commands into rawCmdStack,
            // and binary operators (&& or ||) into symStack.
            for (unsigned int j = 0; j < seq[i].size(); j++)
            {
                // If the char is not & or |, then store it into command.
                if (seq[i][j] != '&' && seq[i][j] != '|')
                    command += seq[i][j];
                else
                {
                    // Check if the following char is the same as current one.
                    // To guarantee parsing && and || correctly.
                    if (seq[i][j] == seq[i][j + 1])
                    {
                        // Finally, use space as the delimiter, to generate the
                        // unary (single) command.
                        unary = tokenize(command, " ");

                        cmdStack.push(new CmdUnary(unary, executor));
                        symStack.push(string(2, seq[i][j]));

                        command = ""; // Clear command for the next one.
                        j++; // Skip the second & or |.
                    }
                }
            }

            // At this point, the last command has not been parsed.
            unary = tokenize(command, " ");
            // Note that the command could be empty or spaces.
            // Consider this: ls -al ; # list all.
            // The command between ; and # is empty.
            if (unary.size() > 0)
                cmdStack.push(new CmdUnary(unary, executor));

            // Note that symStack can be empty at this point,
            // under the circumstance that this token is a single command.
            while (!symStack.empty())
            {
                CmdBinary* binCmd = NULL;

                // The top element of rawCmdStack is the
                // right side of a binary operator, and the element below
                // it is the left side of the operator.
                CmdBase* cmdRight = cmdStack.top();
                cmdStack.pop();
                CmdBase* cmdLeft = cmdStack.top();
                cmdStack.pop();

                // Create a binary command based the operator.
                if (symStack.top() == "&&")
                    binCmd = new CmdAnd(cmdLeft, cmdRight);
                else if (symStack.top() == "||")
                    binCmd = new CmdOr(cmdLeft, cmdRight);
                // Note that there is no else-clause, since only && and ||
                // are pushed into symStack.

                // Push this binary command into cmdStack for later
                // compositing the parent one.
                cmdStack.push(binCmd);
                symStack.pop();
            }

            // If the above loop is executed, there should be one element
            // (CmdBinary) left in cmdStack.
            // Otherwise, it's possible to to have one element (CmdUnary)
            // left in cmdStack.
            // Anyway, we add it into the sequence if cmdStack is not empty.
            if (!cmdStack.empty())
            {
                cmdSeq->addCmd(cmdStack.top());
                cmdStack.pop();
            }
        }

        return cmdSeq;
    }

private:
    /**
     * @brief Split string into tokens.
     * @param string The string to be split.
     * @param delimiter The delimiter.
     * @param ignoreEmptyToken Whether ignore empty tokens, default true.
     * @return A vector of tokens.
     */
    std::vector<std::string> tokenize(std::string string,
                                      std::string delimiter,
                                      bool ignoreEmptyToken = true)
    {
        std::vector<std::string> tokens;
        std::string token;

        size_t start = 0, end = string.find_first_of(delimiter);
        while (end != std::string::npos)
        {
            token = string.substr(start, end - start);

            // If a token is empty and ignoreEmptyToken is true, it would be
            // ignored. Otherwise, push the token into vector.
            if (!(token.empty() && ignoreEmptyToken))
                tokens.push_back(token);

            // Update start with position of the character behind delimiter.
            start = end + 1;
            end = string.find_first_of(delimiter, start);
        }

        // Any tokens behind the last delimiter?
        // Or the delimiter actually does not exist in the string?
        if (start < string.length())
            tokens.push_back(string.substr(start));

        return tokens;
    }
};

#endif
