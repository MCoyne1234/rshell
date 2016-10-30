#ifndef __CMD_SEQUENCE__
#define __CMD_SEQUENCE__

class CmdSequence : public CmdBase
{
    typedef std::vector<CmdBase*> CmdVector;
    typedef CmdVector::iterator   CmdIterator;

private:
    CmdVector cmds;

public:
    ~CmdSequence()
    {
        for (unsigned int i = 0; i < cmds.size(); i++)
            delete cmds[i];
    }

    void addCmd(CmdBase* cmd) { cmds.push_back(cmd); }
    void removeCmd(CmdBase* cmd)
    {
        CmdIterator it = std::find(cmds.begin(), cmds.end(), cmd);
        if (it != cmds.end())
        {
            delete *it;
            cmds.erase(it);
        }
    }
    CmdBase* getChildCmd(int i) const { return cmds.at(i); }

    int execute()
    {
        // Use 1 as the default return value, to denote empty sequence.
        int ret = 1;
        for (unsigned int i = 0; i < cmds.size(); i++)
            ret = cmds[i]->execute();
        
        // Return the exit status of the last command
        return ret;
    }

    std::string toString()
    {
        std::string ret;
        for (unsigned int i = 0; i < cmds.size(); i++)
        {
            ret += cmds[i]->toString();
            if (i != cmds.size() - 1)
                ret += "; ";
        }
        return ret;
    }
};

#endif
