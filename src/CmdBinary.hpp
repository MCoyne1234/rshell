#ifndef __CMD_BINARY__
#define __CMD_BINARY__

//! The base class for binary commands.
class CmdBinary : public CmdBase
{
protected:
    CmdBase *leftCmd, *rightCmd;

protected:
    CmdBinary() : leftCmd(NULL), rightCmd(NULL) {}
    /**
     * @brief Construct a binary command with given left and right command.
     * @param leftCmd The left command.
     * @param rightCmd The right command.
     */
    CmdBinary(CmdBase* leftCmd,
              CmdBase* rightCmd)
            : leftCmd(leftCmd), rightCmd(rightCmd)
    {}

    ~CmdBinary()
    {
        // Avoid memory leak
        delete leftCmd;
        delete rightCmd;
    }

public:
    CmdBase *getLeftCmd() const { return leftCmd; }
    void setLeftCmd(CmdBase *cmd) { leftCmd = cmd; }

    CmdBase *getRightCmd() const { return rightCmd; }
    void setRightCmd(CmdBase *cmd) { rightCmd = cmd; }

    // In fact, we don't need rewrite these two methods.
    virtual int execute() = 0;
    virtual std::string toString() = 0;
};

//! The class for operator &&.
class CmdAnd : public CmdBinary
{
public:
    CmdAnd() {}
    CmdAnd(CmdBase* leftCmd,
           CmdBase* rightCmd)
            : CmdBinary(leftCmd, rightCmd)
    {}

    int execute()
    {
        int status = leftCmd->execute();
        if (status) return 0;
        return rightCmd->execute();
    }

    std::string toString()
    {
        return leftCmd->toString() + " && " + rightCmd->toString();
    }
};

//! The class for operator ||.
class CmdOr : public CmdBinary
{
public:
    CmdOr() {}
    CmdOr(CmdBase* leftCmd,
          CmdBase* rightCmd)
            : CmdBinary(leftCmd, rightCmd)
    {}

    int execute()
    {
        int status = leftCmd->execute();
        if (!status) return 0;
        return rightCmd->execute();
    }

    std::string toString()
    {
        return leftCmd->toString() + " || " + rightCmd->toString();
    }
};

#endif
