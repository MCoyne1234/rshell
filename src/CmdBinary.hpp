#ifndef __CMD_BINARY__
#define __CMD_BINARY__

class CmdBinary : public CmdBase
{
protected:
    CmdBase *leftCmd, *rightCmd;

public:
    CmdBase *getLeftCmd() const { return leftCmd; }
    void setLeftCmd(CmdBase *cmd) { leftCmd = cmd; }

    CmdBase *getRightCmd() const { return rightCmd; }
    void setRightCmd(CmdBase *cmd) { rightCmd = cmd; }

    virtual int execute() = 0;
};

class CmdAnd : public CmdBinary
{
public:
    int execute()
    {
        int status = leftCmd->execute();
        if (status) return 0;
        return rightCmd->execute();
    }
};

class CmdOr : public CmdBinary
{
public:
    int execute()
    {
        int status = leftCmd->execute();
        if (!status) return 0;
        return rightCmd->execute();
    }
};

#endif
