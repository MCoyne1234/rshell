#ifndef __CMD_BASE__
#define __CMD_BASE__

class CmdBase
{
public:
    virtual ~CmdBase() {}

    virtual int execute() = 0;
    virtual std::string toString() = 0;
};

#endif
