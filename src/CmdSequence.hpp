#ifndef __CMD_SEQUENCE__
#define __CMD_SEQUENCE__

class CmdSequence : public CmdBase
{
    typedef std::vector<CmdBase*> CmdVector;
    typedef CmdVector::const_iterator CmdIterator;

private:
    CmdVector cmds;

public:
    ~CmdSequence()
    {
        for (int i = 0; i < cmds.size(); i++)
        {
#ifdef DEBUG
            std::cout << "Deleting " << cmds[i]->toString() << std::endl;
#endif
            delete cmds[i];
        }
    }

    void addCmd(CmdBase* cmd) { cmds.push_back(cmd); }
    void removeCmd(CmdBase* cmd)
    {
        CmdIterator it = std::find(cmds.begin(), cmds.end(), cmd);
        if (it != cmds.end())
        {
#ifdef DEBUG
            std::cout << "Deleting " << (*it)->toString() << std::endl;
#endif
            delete *it;
            cmds.erase(it);
        }
    }
    CmdBase* getChildCmd(int i) const { return cmds.at(i); }

    int execute()
    {
        int ret = 0;
        for (int i = 0; i < cmds.size(); i++)
            ret = cmds[i]->execute();
        
        // Return the exit status of the last command
        return ret;
    }

    std::string toString()
    {
        std::string ret;
        for (int i = 0; i < cmds.size(); i++)
        {
            ret += cmds[i]->toString();
            if (i != cmds.size() - 1)
                ret += "; ";
        }
        return ret;
    }
};

#endif
