#include "AgentBuilder.h"
using namespace std;

int AgentBuilder::num_agents = 0;

AgentBuilder::AgentBuilder(): t_init(1000), t_round(50), game_config(nullptr), agent_cmdlines(nullptr){

}

AgentBuilder::AgentBuilder(map<string, string>* game_config, vector<string>* agent_cmdline): t_init(1000), t_round(50), game_config(game_config), agent_cmdlines(agent_cmdline){

}

void AgentBuilder::config(int t_init, int t_round){
    this->t_init = t_init;
    this->t_round = t_round;
}

bool AgentBuilder::genAgents(vector<AgentInterface*>& agents){
    if (agent_cmdlines == nullptr) return false;
    for (string cmdline: *agent_cmdlines){
        if (!getAgent(cmdline, agents)) return false;
        else{
            if (debug_mode) cout << "Initialized Child: " << cmdline << endl;
        }
    }
    
    //Busy loop to ensure all agent processes starts and are stopped by SIGSTOP
    bool agents_ready = false;
    bool agents_error = false;
    while (!agents_ready && !agents_error){
        agents_ready = false;
        agents_error = false;
        for (AgentInterface* agent: agents){
            if (!agent->running()){
                agents_error = true;
                //cout << "Agent: " << agent->getPid() << " NOT RUNNING|" << agent->getCmd();
            }
            if (!agent->stopped()){
                agents_ready = false;
                //cout << "Agent: " << agent->getPid() << " NOT STOPPED|" << agent->getCmd();
            }
        }
    }
    if (agents_error){
        cout << "Initialization Error: Agents compilation error!" << endl;
        return false;
    }
    return true;
}

bool AgentBuilder::getAgent(string cmdline, vector<AgentInterface*>& agents){
    //Starts our process and builds our Agent for us
    int fd_child_in[2], fd_child_out[2];
    pipe(fd_child_in);
    pipe(fd_child_out);
    string fname = to_string(num_agents) + "_agent.log";
    int ferr = open(fname.c_str(), O_FLAGS, S_FLAGS);
    num_agents++;
    
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
        dup2(ferr, fileno(stderr));
        close(fd_child_in[0]); close(fd_child_in[1]);
        close(fd_child_out[0]); close(fd_child_out[1]);
        close(ferr);
        
        write(STDOUT_FILENO, &magic, 1);

        if (execvp(arg[0], arg) < 0){
            //Error has occurred
            exit(1);
        }
    }
    else{
        //Parent Process
        sigprocmask(SIG_BLOCK, &mask_all, NULL);        //Block all incoming signals (thread-safe)

        //Ensure we do all necessary setup steps before handling any SIGCHLDs
        //Do not close pipes as other end needs to be open for children to access
        new_agent = new AgentInterface(t_init, t_round);
        new_agent->attach_pipes(fd_child_out[0], fd_child_in[1]);
        new_agent->sig_callback(0);
        new_agent->set_running(true);
        new_agent->set_stopped(false);
        new_agent->setPid(pid);
        new_agent->setCmd(cmdline);
        agents.push_back(new_agent);    //Modifies common datastruc (that sigchld_handler relies on)
       
        //sigprocmask(SIG_SETMASK, &prev_one, NULL);      //Re-enable signals (avoids race conditions)
        
        //Synchronize two processes
        read(fd_child_out[0], &magic, 1);
        
        //PAUSE our process until Referee calls for it
        //int child_status;
        kill(-pid, SIGSTOP);
        //waitpid(pid, &child_status, WUNTRACED);
        /*if (WIFEXITED(child_status)){
            new_agent->sig_callback(WEXITSTATUS(child_status), false);
            sigprocmask(SIG_SETMASK, &prev_one, NULL);      //Re-enable signals (avoids race conditions)
            return false;
        }
        else if (WIFSIGNALED(child_status)){
            new_agent->sig_callback(WTERMSIG(child_status), false);
            sigprocmask(SIG_SETMASK, &prev_one, NULL);      //Re-enable signals (avoids race conditions)
            return false;
        }*/
        sigprocmask(SIG_SETMASK, &prev_one, NULL);      //Re-enable signals (avoids race conditions)
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
