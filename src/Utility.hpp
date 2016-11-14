#ifndef __UTILITY__
#define __UTILITY__

//! Get the name of current logged in user.
string getCurrentUserName()
{
    char* userName = getlogin();
    if (userName == NULL)
        perror(SHELL_NAME ": getlogin()");

    return userName ? string(userName) : "??name??";
}

//! Get the host name.
string getCurrentHostName()
{
    char hostName[256];

    int ret = gethostname(hostName, 256);
    if (ret < 0)
        perror(SHELL_NAME ": gethostname()");

    return ret >= 0 ? string(hostName) : "??host??";
}

//! Get the current absolute working directory.
string getCurrentAbsolutePath()
{
    char currentDir[1024];

    char* ret = getcwd(currentDir, 1024);
    if (ret == NULL)
        perror(SHELL_NAME ": getcwd()");

    return ret != NULL ? string(currentDir) : "??dir??";
}

//! Get the current relative working directory.
string getCurrentRelativePath()
{
    string absPath = getCurrentAbsolutePath();
    string::size_type lastSlashPos = absPath.find_last_of("/");

    // When getcwd() failed, there is no `/` in the absolute path.
    if (lastSlashPos == string::npos)
        return absPath;
    else
    {
        // This occurred only when absolute path is `/`.
        if (lastSlashPos == absPath.length() - 1)
            return absPath;
        else
            return absPath.substr(lastSlashPos + 1);
    }
}

/**
 * @brief Return the prompt with current logined user name, host name and
 *        working directory, such as `example@hammer.cs.ucr.edu: etc $`.
 */
string getPromptInfo()
{
    return getCurrentUserName() + "@" + getCurrentHostName() + ": " +
           getCurrentRelativePath() + " $ ";
}

#endif
