#include "AgentBuilder.h"
using namespace std;

AgentBuilder::AgentBuilder(){

}

bool AgentBuilder::getAgent(string cmdline, vector<AgentInterface*>& agents){
    //Starts our process and builds our Agent for us
    int fd_child_in[2], fd_child_out[2];
    pipe(fd_child_in);
    pipe(fd_child_out);
    AgentInterface* new_agent = nullptr;

    //Parse cmdline
    parseline(cmdline.c_str(), arg);
    
    sigset_t mask_all, mask_one, prev_one;
    sigfillset(&mask_all);
    sigemptyset(&mask_one);
    sigaddset(&mask_one, SIGCHLD);
    pid_t pid;
    char magic = '!';

    sigprocmask(SIG_BLOCK, &mask_one, &prev_one);
    if ((pid = fork()) == 0){
        //Child Process
        sigprocmask(SIG_SETMASK, &prev_one, NULL);      //Re-enable all child signals
        setpgid(0, 0);

        dup2(fd_child_in[0], STDIN);
        dup2(fd_child_out[1], STDOUT);
        close(fd_child_in[0]); close(fd_child_in[1]);
        close(fd_child_out[0]); close(fd_child_out[1]);
        
        write(STDOUT_FILENO, &magic, 1);

        execve(arg[0], arg, NULL);
    }
    else{
        //Parent Process
        sigprocmask(SIG_BLOCK, &mask_all, NULL);        //Block all incoming signals (thread-safe)

        //Ensure we do all necessary setup steps before handling any SIGCHLDs
        //Do not close pipes as other end needs to be open for children to access
        new_agent = new AgentInterface();
        new_agent->attach_pipes(fd_child_out[0], fd_child_in[1]);
        new_agent->sig_callback(0, true);
        new_agent->setPid(pid);
        new_agent->setCmd(cmdline);
        agents.push_back(new_agent);    //Modifies common datastruc (that sigchld_handler relies on)
       
        sigprocmask(SIG_SETMASK, &prev_one, NULL);      //Re-enable signals (avoids race conditions)
        
        //Synchronize two processes
        read(fd_child_out[0], &magic, 1);
        
        //PAUSE our process until Referee calls for it
        kill(-pid, SIGSTOP);
        waitpid(pid, NULL, WUNTRACED);
    }
    return true;
}

AgentBuilder::~AgentBuilder(){

}

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
void AgentBuilder::parseline(const char* cmdline, char** argv){
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */

    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
    buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
        buf++;
        delim = strchr(buf, '\'');
    }
    else {
        delim = strchr(buf, ' ');
    }

    while (delim) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* ignore spaces */
               buf++;

        if (*buf == '\'') {
            buf++;
            delim = strchr(buf, '\'');
        }
        else {
            delim = strchr(buf, ' ');
        }
    }
    argv[argc] = NULL;
    return;    
}
