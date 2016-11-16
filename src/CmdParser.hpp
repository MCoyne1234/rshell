#ifndef __CMD_PARSER__
#define __CMD_PARSER__

//! The class for parsing user input into command tree.
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
     *     - CmdBinary
     *       - CmdUnary
     *       - CmdUnary
     *     - CmdUnary
     *
     * When syntax error occured, such as missing command before or after &&,
     * we will print corrsopnding information and return NULL.
     *
     * @param input The user input.
     * @param executor The executor used in tree leaf CmdUnary.
     * @return The root of the parse tree (even an empty sequence is parsed),
     *         or NULL on bad syntax.
     */
    CmdBase* parse(std::string input,
                   Executor* executor = NULL)
    {
        // The return value is a CmdSequence, because the input could always
        // be parsed into a sequence, even though only one command.
        CmdSequence* cmdSeq = new CmdSequence();
        
        // Use # as the delimiter, to retrieve the statement before #.
        std::string stmt = input.substr(0, input.find_first_of("#"));
        
        // Working on a stream is more convenient.
        std::istringstream stream(stmt);
        std::string token;
        char nextChar = ' ';
        
        // Stacks help process different situations.
        std::stack<CmdBase*> cmdStack;
        std::stack<char> orderStack;
        std::stack<char> quoteStack;
        
        // Store the command being processed currently.
        CmdUnary* curCmd = new CmdUnary();
        
        // Scan through the whole string
        for (; ; nextChar = stream.get())
        {
            // We first process quoted string.
            if (!quoteStack.empty())
            {
                if (nextChar != quoteStack.top())
                    token += nextChar;
                else
                    quoteStack.pop();
            }
            else
            {
                
            }
        }
        
        /*
        // Secondly, use semi-colon as the delimiter, to generate a sequence.
        std::vector<std::string> seq = tokenize(stmt, ";");
        
        for (unsigned int i = 0 ; i < seq.size(); i++)
        {
            // Thirdly, we use two queues to parse binary commands.
            std::queue<std::string> symQueue;
            std::deque<CmdBase*> cmdQueue;
            std::string command;
            std::vector<std::string> unary;

            // Iterate all characters, push commands into cmdQueue,
            // and binary operators (&& or ||) into symQueue.
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
                        // unary (single) command. This command is the one
                        // before the binary operator (left side).
                        unary = tokenize(command, " \t");

                        // Empty command? Such as `&& echo 123` or a more
                        // complex `echo 123 && || mkdir test`
                        if (unary.size() == 0)
                        {
                            std::cerr << SHELL_NAME ": bad syntax: ";
                            std::cerr << "missing command before `";
                            std::cerr << string(2, seq[i][j]) << "`";
                            std::cerr << std::endl;

                            releaseQueue<CmdBase>(cmdQueue);
                            delete cmdSeq;
                            return NULL;
                        }
                        else
                        {
                            cmdQueue.push_back(new CmdUnary(unary, executor));
                            symQueue.push(string(2, seq[i][j]));

                            command = ""; // Clear command for the next one.
                            j++; // Skip the second & or |.
                        }
                    }
                    else
                    {
                        // Currently, our syntax requires there must be
                        // another & (or |) followed by the first one.
                        std::cerr << SHELL_NAME ": bad syntax: ";
                        std::cerr << "unexpected token `" << seq[i][j] << "`";
                        std::cerr << std::endl;

                        releaseQueue<CmdBase>(cmdQueue);
                        delete cmdSeq;
                        return NULL;
                    }
                }
            }

            // At this point, the last command has not been parsed.
            unary = tokenize(command, " \t");
            // Note that the command could be empty or spaces.
            // Consider this: ls -al ; # list all.
            // The command between ; and # is empty.
            if (unary.size() > 0)
                cmdQueue.push_back(new CmdUnary(unary, executor));

            // Check whether the last command is missing, such as a sequence
            // `echo 123 && mkdir test ||`.
            // For situation like `echo 123 && || mkdir test` has already
            // been handle before.
            // Note that we have to check whether symQueue is empty first.
            if (!symQueue.empty() && cmdQueue.size() - symQueue.size() != 1)
            {
                std::cerr << SHELL_NAME ": bad syntax: ";
                std::cerr << "missing command after `";
                std::cerr << symQueue.front() << "`" << std::endl;

                releaseQueue<CmdBase>(cmdQueue);
                delete cmdSeq;
                return NULL;
            }

            // Note that symQueue can be empty at this point,
            // under the circumstance that this token is a single command.
            while (!symQueue.empty())
            {
                CmdBinary* binCmd = NULL;

                CmdBase* cmdLeft = cmdQueue.front();
                cmdQueue.pop_front();
                CmdBase* cmdRight = cmdQueue.front();
                cmdQueue.pop_front();

                // Create a binary command based the operator.
                if (symQueue.front() == "&&")
                    binCmd = new CmdAnd(cmdLeft, cmdRight);
                else if (symQueue.front() == "||")
                    binCmd = new CmdOr(cmdLeft, cmdRight);
                // Note that there is no else-clause, since only && and ||
                // are pushed into symQueue.

                // Push this binary command into cmdQueue for later
                // compositing the parent one.
                // Note that it has to be put in the front of the cmdQueue,
                // since the order of executing binary operators is
                // left to right.
                cmdQueue.push_front(binCmd);
                symQueue.pop();
            }

            // If the above loop is executed, there should be one element
            // (CmdBinary) left in cmdQueue.
            // Otherwise, it's possible to to have one element (CmdUnary)
            // left in cmdQueue.
            // Anyway, we add it into the sequence if cmdQueue is not empty.
            if (!cmdQueue.empty())
            {
                cmdSeq->addCmd(cmdQueue.front());
                cmdQueue.pop_front();
            }
        }
         */

        return cmdSeq;
    }

private:
    /**
     * @brief Release allocated memory in a queue.
     * @tparam ValueT The value type of pointers in the queue.
     * @param[in,out] queue The queue to be released.
     */
    template <class ValueT>
    void releaseQueue(std::deque<ValueT*> &queue)
    {
        while (!queue.empty())
        {
            delete queue.front();
            queue.pop_front();
        }
    }

    //! Print error with prefix "rshell: bad syntax: ".
    void printSyntaxError(std::string format, ...)
    {
        va_list args;
        va_start(args, format);
        
        std::cerr << SHELL_NAME ": bad syntax: ";
        vfprintf(stderr, format.c_str(), args);
        std::cerr << std::endl;
    }
};

#endif
