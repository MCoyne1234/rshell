#ifndef __CMD_BINARY__
#define __CMD_BINARY__

class CmdBinary : public CmdBase
{
protected:
    CmdBase *leftCmd, *rightCmd;

public:
    CmdBinary() {}
    CmdBinary(CmdBase* leftCmd,
              CmdBase* rightCmd)
            : leftCmd(leftCmd), rightCmd(rightCmd)
    {}

    ~CmdBinary()
    {
#ifdef DEBUG
        std::cout << "Deleting " << leftCmd->toString() << std::endl;
        std::cout << "Deleting " << rightCmd->toString() << std::endl;
#endif
        delete leftCmd;
        delete rightCmd;
    }

    CmdBase *getLeftCmd() const { return leftCmd; }
    void setLeftCmd(CmdBase *cmd) { leftCmd = cmd; }

    CmdBase *getRightCmd() const { return rightCmd; }
    void setRightCmd(CmdBase *cmd) { rightCmd = cmd; }

    virtual int execute() = 0;
    virtual std::string toString() = 0;
};

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
        return leftCmd->toString() + " && " + rightCmd->toString();
    }
};

#endif
