#ifndef __CMD_SEQUENCE__
#define __CMD_SEQUENCE__

//! The class for command sequence sparaterd by semi-colon (;).
class CmdSequence : public CmdBase
{
    typedef std::vector<CmdBase*> CmdVector;
    typedef CmdVector::iterator   CmdIterator;

private:
    CmdVector cmds;

public:
    ~CmdSequence()
    {
        // Avoid memory leak
        for (unsigned int i = 0; i < cmds.size(); i++)
            delete cmds[i];
    }

    //! Add a command into the sequence.
    void addCmd(CmdBase* cmd) { cmds.push_back(cmd); }
    //! Remove a given command in the sequence if exists.
    void removeCmd(CmdBase* cmd)
    {
        CmdIterator it = std::find(cmds.begin(), cmds.end(), cmd);
        if (it != cmds.end())
        {
            // Release the element, avoiding memory leak.
            delete *it;
            cmds.erase(it);
        }
    }
    //! Get the i-th command in the sequnce.
    CmdBase* getChildCmd(int i) const { return cmds.at(i); }

    int execute()
    {
        // Use 0 as the default return value, to denote empty sequence.
        int ret = 0;
        for (unsigned int i = 0; i < cmds.size(); i++)
            ret = cmds[i]->execute();
        
        // Return the exit status of the last command
        return ret;
    }
};

#endif
