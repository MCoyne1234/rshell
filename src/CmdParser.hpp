#ifndef __CMD_PARSER__
#define __CMD_PARSER__

//! The class for parsing user input into command tree.
class CmdParser
{
    // On opQueue, we use `c` to denote a command.
    // Note that opCmd would only be enqueued by &&, ||, ), ; and EOF.
#define OP_CMD ('c')
    
private:
    /**
     * @brief Release allocated memory in a queue.
     * @tparam ValueT The value type of pointers in the queue.
     * @param[in,out] queue The queue to be released.
     */
    template <class ValueT>
    void releaseQueue(std::deque<ValueT*>& queue)
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
    
    /** 
     * @brief Generate a CmdSequence based on given cmdQueue and opQueue
     *        from a start iterator in opQueue to the end of opQueue.
     *
     * Note that, at the point of calling this function, there should be
     * at most one ) in opQueue.
     */
    void generateCmdSeq(std::deque<CmdBase*>& cmdQueue,
                        std::deque<char>& opQueue,
                        std::deque<char>::iterator opStart)
    {
        CmdSequence* cmdSeq = new CmdSequence();
        
        // Cut off the opQueue from opStart.
        std::deque<char> partOpQueue(opStart, opQueue.end());
        opQueue.erase(opStart, opQueue.end());
        
        // Count commands in opQueue, for cutting off cmdQueue.
        std::ptrdiff_t cmds =
            std::count(partOpQueue.begin(), partOpQueue.end(), OP_CMD);
        // The count is supposed to be greater than 0.
        assert(cmds > 0);
        // Cut off the cmdQueue.
        std::deque<CmdBase*>::iterator cmdStart =
            cmdQueue.begin() + (cmdQueue.size() - cmds);
        std::deque<CmdBase*> partCmdQueue(cmdStart, cmdQueue.end());
        cmdQueue.erase(cmdStart, cmdQueue.end());
        
        // Walkthrough the part of opQueue.
        while (!partOpQueue.empty())
        {
            char front = partOpQueue.front();
            partOpQueue.pop_front();
        
            // && and ||
            if (front == '&' || front == '|')
            {
                // Fetch the commands on the both sides.
                CmdBase* leftCmd = partCmdQueue.front();
                partCmdQueue.pop_front();
                CmdBase* rightCmd = partCmdQueue.front();
                partCmdQueue.pop_front();
                
                // Construct the binary command.
                CmdBinary* binCmd = NULL;
                if (front == '&')
                    binCmd = new CmdAnd(leftCmd, rightCmd);
                else // front == '|'
                    binCmd = new CmdOr(leftCmd, rightCmd);
                partCmdQueue.push_front(binCmd);
            }
            // ;
            else if (front == ';')
            {
                cmdSeq->addCmd(partCmdQueue.front());
                partCmdQueue.pop_front();
            }
            // )
            else if (front == ')')
            {
                // If we reached ), the partOpQueue shall be empty,
                // to denote this part has ended.
                assert(partOpQueue.empty());
                
                // Transfer the front of partCmdQueue into retSeq.
                cmdSeq->addCmd(partCmdQueue.front());
                partCmdQueue.pop_front();
            }
            // Note that, `(` is skipped.
        }
        
        // partCmdQueue may be not empty.
        if (!partCmdQueue.empty())
        {
            // But at this point, its size should be 1.
            assert(partCmdQueue.size() == 1);
            cmdSeq->addCmd(partCmdQueue.front());
        }
        
        // Push necessary elements into original queues.
        opQueue.push_back(OP_CMD);
        cmdQueue.push_back(cmdSeq);
    }
    
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
#pragma GCC diagnostic push
        // We have to use variadic macros, however it is a feature of C99,
        // which would throw warning. Thus, we disable this warning
        // in this function (CmdParser::parse).
#pragma GCC diagnostic ignored "-Wvariadic-macros"

#define THROW_SYNTAX_ERROR(...)                         \
            do {                                        \
                printSyntaxError(__VA_ARGS__);          \
                releaseQueue<CmdBase>(cmdQueue);        \
                delete cmdUnary;                        \
                return NULL;                            \
            } while(0)
        
#define PUSH_ARGUMENT                                   \
            do {                                        \
                if (!token.empty())                     \
                    cmdUnary->pushArg(token);           \
                token = "";                             \
            } while(0)
        
#define ENQUEUE_UNARY_COMMAND                           \
            do {                                        \
                if (!cmdUnary->isArgListEmpty()) {      \
                    cmdQueue.push_back(cmdUnary);       \
                    opQueue.push_back(OP_CMD);          \
                    cmdUnary = new CmdUnary();          \
                    cmdUnary->setExecutor(executor);    \
                }                                       \
            } while (0)
                
        // Use # as the delimiter, to retrieve the statement before #.
        std::string stmt = input.substr(0, input.find_first_of("#"));
        
        // Working on a stream is more convenient.
        std::istringstream stream(stmt);
        std::string token;
        
        // Queue for storing commands.
        std::deque<CmdBase*> cmdQueue;
        // Queue for processing operators (precedence, binary and semi-colon).
        std::deque<char> opQueue;
        // The quoteStack is suppposed to hold at most 1 element.
        std::stack<char> quoteStack;
        
        // Store the unary command being processed currently.
        CmdUnary* cmdUnary = new CmdUnary();
        cmdUnary->setExecutor(executor);
        
        // Scan through the whole string.
        for (char nextChar = stream.get(); ; nextChar = stream.get())
        {
            // First, consider EOF
            if (nextChar == EOF)
            {
                // Missing the right quotation mark.
                if (!quoteStack.empty())
                    THROW_SYNTAX_ERROR("missing closing `%c`",
                                       quoteStack.top());
                
                // Push the last token.
                PUSH_ARGUMENT;
                // At this point, if the back of opQueue is ),
                // then there should not be any command after that.
                if ((!opQueue.empty() && opQueue.back() == ')') &&
                    !cmdUnary->isArgListEmpty())
                    THROW_SYNTAX_ERROR("unexpected token `%s`",
                                       token.c_str());
                
                // At this point, the last command may not be enqueued.
                ENQUEUE_UNARY_COMMAND;
                // Since ENQUEUE_UNARY_COMMAND would create a new cmdUnary,
                // we have to delete it to avoid memory leak.
                delete cmdUnary;
                cmdUnary = NULL;
                
                // The back of opQueue should not be binary operators.
                if (!opQueue.empty() &&
                    (opQueue.back() == '&' || opQueue.back() == '|')
                    )
                    THROW_SYNTAX_ERROR("unexpected token `%c%c`",
                                       opQueue.back(), opQueue.back());
                
                // Still exists opening parentese?
                if (std::find(opQueue.begin(), opQueue.end(), '(') !=
                    opQueue.end())
                    THROW_SYNTAX_ERROR("missing closing `)`");
                
                // Build the final sequence.
                generateCmdSeq(cmdQueue, opQueue, opQueue.begin());
                
                // We can safely break here.
                break;
            }
            // We then process quoted string.
            else if (!quoteStack.empty())
            {
                // Haven't met the closing quotation mark.
                if (nextChar != quoteStack.top())
                    token += nextChar;
                else
                {
                    PUSH_ARGUMENT;
                    quoteStack.pop();
                }
            }
            // Finally, other situations.
            else
            {
                // Spaces or tabs
                if (nextChar == ' ' || nextChar == '\t')
                    PUSH_ARGUMENT;
                // Quotation mark
                else if (nextChar == '\'' || nextChar  == '\"')
                {
                    // At this point, the back of opQueue should not be ).
                    if (!opQueue.empty() && opQueue.back() == ')')
                        THROW_SYNTAX_ERROR("unexpected token `%c`", nextChar);
                    
                    // At this point, the quoteStack is supposed to be empty,
                    // if, of course, the code was written correctly.
                    assert(quoteStack.empty());
                    quoteStack.push(nextChar);
                }
                // Left parenthese
                else if (nextChar == '(')
                {
                    PUSH_ARGUMENT;
                    
                    // At this point, the back of opQueue should not be
                    // right parentese, and cmdUnary should be empty.
                    if (!cmdUnary->isArgListEmpty() ||
                        (!opQueue.empty() && opQueue.back() == ')'))
                        THROW_SYNTAX_ERROR("unexpected token `(`");
                    else
                        opQueue.push_back(nextChar);
                    
                    // Note that we needn't enqueue the unary command here.
                    // It is supposed to be empty.
                }
                // Right parentese
                else if (nextChar == ')')
                {
                    // Find corresponding left parenthese,
                    // that should be the last one.
                    std::deque<char>::reverse_iterator left =
                        std::find(opQueue.rbegin(), opQueue.rend(), '(');
                    
                    if (left == opQueue.rend())
                        THROW_SYNTAX_ERROR("unexpected token `)`");
                    
                    // The last command before ) may not be enqueued.
                    PUSH_ARGUMENT;
                    ENQUEUE_UNARY_COMMAND;
                    
                    // At this point, the opQueue should not be empty,
                    // and the back of opQueue should not be
                    // binary operators.
                    if (opQueue.empty()       ||
                        opQueue.back() == '&' ||
                        opQueue.back() == '|')
                        THROW_SYNTAX_ERROR("unexpected token `%c%c`",
                                           opQueue.back(), opQueue.back());

                    // Since bash doesn't accept (), we do the same thing.
                    if (opQueue.back() == '(')
                        THROW_SYNTAX_ERROR("unexpected token `)`");
                    
                    opQueue.push_back(nextChar);
                    
                    // Convert reverse iterator to an iterator.
                    // Note that since the reverse iterator points to the next
                    // element of its base, so we have to minus 1 manually.
                    std::deque<char>::iterator start = --(left.base());
                    
                    // Generate the command sequence among this ().
                    generateCmdSeq(cmdQueue, opQueue, start);
                }
                // Binary operators
                else if (nextChar == '&' || nextChar == '|')
                {
                    char curChar = nextChar;
                    nextChar = stream.get();
                    
                    // Another & or | is required.
                    if (curChar != nextChar)
                        THROW_SYNTAX_ERROR("unexpected token `%c`", nextChar);
                    
                    PUSH_ARGUMENT;
                    // At this point, cmdUnary should not be empty.
                    if (cmdUnary->isArgListEmpty())
                        THROW_SYNTAX_ERROR("unexpected token `%c%c`",
                                           nextChar, nextChar);
                    
                    // Push necessary elements into queues.
                    ENQUEUE_UNARY_COMMAND;
                    opQueue.push_back(nextChar);
                }
                // Semi-colon
                else if (nextChar == ';')
                {
                    // The last command before ; may not be enqueued.
                    PUSH_ARGUMENT;
                    ENQUEUE_UNARY_COMMAND;
                    
                    // Since bash doesn't accept empty statement, so do we
                    // What's more, the operator before ; must be a command.
                    if (opQueue.empty() || opQueue.back() != OP_CMD)
                        THROW_SYNTAX_ERROR("unexpected token `;`");
                    /*
                    // At this point, the back of opQueue should not be
                    // binary operators or (.
                    if (opQueue.back() == '&' ||
                        opQueue.back() == '|')
                        THROW_SYNTAX_ERROR("unexpected token `%c%c`",
                                           opQueue.back(), opQueue.back());
                    if (opQueue.back() == '(')
                        THROW_SYNTAX_ERROR("unexpected token `(`");
                    */
                     
                    opQueue.push_back(nextChar);
                }
                // For other characters, stores them into token.
                else
                    token += nextChar;
            }
        }

        assert(cmdQueue.size() == 1);
        return cmdQueue.front();
#undef THROW_SYNTAX_ERROR
#undef PUSH_ARGUMENT
#undef ENQUEUE_UNARY_COMMAND

#pragma GCC diagnostic pop
    }

#undef OP_CMD
};

#endif
