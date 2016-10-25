#ifndef __CMD_SEQUENCE__
#define __CMD_SEQUENCE__

class CmdSequence : public CmdBase
{
    typedef std::vector<CmdBase*>::const_iterator CmdIterator;

private:
    std::vector<CmdBase*> cmds;

public:
    void addCmd(CmdBase* cmd) { cmds.push_back(cmd); }
    void removeCmd(CmdBase* cmd)
    {
        CmdIterator it = std::find(cmds.begin(), cmds.end(), cmd);
        if (it != cmds.end())
            cmds.erase(it);
    }
    CmdBase *getChildCmd(int i) const { return cmds.at(i); }

    int execute()
    {
        int ret = 0;
        for (int i = 0; i < cmds.size(); i++)
            ret = cmds[i]->execute();
        
        // Return the exit status of the last command
        return ret;
    }
};

#endif
