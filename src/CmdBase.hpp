#ifndef __CMD_BASE__
#define __CMD_BASE__

//! The abstract base class for commands.
class CmdBase
{
public:
    virtual ~CmdBase() {}

    //! Execute corresponding command(s).
    virtual int execute() = 0;
};

#endif
