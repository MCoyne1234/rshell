#ifndef __EXECUTOR__
#define __EXECUTOR__

#include <sys/stat.h> 
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <vector>

class Executor
{
private:
    bool exitExecuted;

public:
    Executor() : exitExecuted(false) {}

    bool isExitExecuted() const { return exitExecuted; }

    int execute(std::string executable,
                std::vector<std::string> argList)
    {
        if (executable == "exit")
        {
            exitExecuted = true;
            return 0;
        }
        else
        {
            // TODO: Invoke system calls
            
            std::cout << executable<<endl;
            for (unsigned int i = 0; i < argList.size(); i++) {
                std::cout << " " << argList[i];
            //std::cout << endl;
            }
            if (strcmp(executable.c_str(),"cd")==0) {
                if(argList.size()==2) {
                    struct stat dest;
                    if(stat(argList.at(0).c_str(),&dest)==-1)
                    {
                        perror("The following error occurred: No such file or directory");
                    
                    }
                }
            
            }
            
            else {
                //argList.push_front(executable);
                int x=fork();
                
                if(x==0) {
                
                    vector<string> argLs;
                    argLs.at(0)=executable;
                
                    for(int w=0; w<argList.size();++w) {
                        argLs.push_back(argList.at(w));
                    }
                
                    std::vector<char *> argv(argLs.size() + 1);    // one extra for the null
                
                    for(std::size_t i = 0; i != argLs.size(); ++i) {
                    
                        argv[i] = &argLs[i][0];
                    }
                
                    if(execvp(argv[0], argv.data()) == -1) {
	  	                perror("Error with execvp()!");
		                exit(1);
                    }
	                exit(1);
	            }
	            
		        else if(x == -1 || x < 0) {
		            perror("fork");
			        exit(1);
			    }
			    
		   	    else {
			        if (wait(0) == -1) {
			            perror("Error with wait()!");
			        }
    		    }
            }
        }
        return 0;
    }
    
};

#endif