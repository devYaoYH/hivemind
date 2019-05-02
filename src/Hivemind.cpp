// Hivemind.cpp : Defines the entry point for the console application.

#include "includes.h"
#include "AgentBuilder.h"
#include "AgentInterface.h"

char* arg[MAX_ARGS];
//char test_prog[] = "timed_child";
char test_prog[] = "/usr/bin/python";
char py_script[] = "timed_py.py";
char* test_args[] = {py_script};

vector<AgentInterface*> agents;

//Signal Handler
void sigchld_handler(int sig);
__sighandler_t Signal(int signum, __sighandler_t handler);

//Process Control Methods
void remove_child(pid_t pid, int child_status, bool terminated);

int main(int argc, char* argv[])
{
    //Install signal handler for child processes
    Signal(SIGCHLD, sigchld_handler);
    
    //Initiate a Builder class for us to generate Agent objects from config file
    AgentBuilder builder = AgentBuilder();

    /* Mini Test Area, build one first */
    //Test Write to child
    string test_msg = "hey there\n";
    string end = "quit\n";
    string msg = "hello world\n";
    
    builder.getAgent("/usr/bin/python timed_py.py\n", agents);
    builder.getAgent("/usr/bin/java SimpleIO\n", agents);
    builder.getAgent("timed_child\n", agents);
    
    //PAUSE ALL AGENTS
    for (AgentInterface* test_agent: agents){
        kill(-(test_agent->getPid()), SIGSTOP);
        waitpid(test_agent->getPid(), NULL, WUNTRACED);
    }

    //Dummy Interaction with child script
    for (AgentInterface* test_agent: agents){
        kill(-(test_agent->getPid()), SIGCONT);
        waitpid(test_agent->getPid(), NULL, WCONTINUED);
        string response;
        const int* pipes = test_agent->getFd();
        cout << "[PARENT] READING FROM: " << pipes[0] << "|WRITING TO: " << pipes[1] << endl;
        
        for(int i=0;i<10;++i){
            *test_agent << msg;
            *test_agent >> response;
            cout << response << endl;
        }
        kill(-(test_agent->getPid()), SIGSTOP);
        waitpid(test_agent->getPid(), NULL, WUNTRACED);
    }

    for (AgentInterface* test_agent: agents){
        if (test_agent->running()){
            cout << test_agent->getCmd();
        }
    }

    for (AgentInterface* test_agent: agents){
        kill(-(test_agent->getPid()), SIGCONT);
        waitpid(test_agent->getPid(), NULL, WCONTINUED);
        string response;
        *test_agent << end;
        *test_agent >> response;
        cout << response << endl;
    }

    //Loop through agents and wait till execution terminates
    bool complete = false;
    while(!complete){
        complete = true;
        for (AgentInterface* agent: agents){
            if (agent->running()){
                complete = false;
            }
        }
    }
    cout << "Children closed successfully: " << complete << endl;
    /* End Test Region */
    
    //Start GameManager Referee Object
    
    //TODO: Waits till GameManager terminates

    return 0;
}

/* Process Methods */
void remove_child(pid_t pid, int child_status, bool terminated){
    char s[] = "child_signaled\n";
    write(STDOUT_FILENO, s, 15);
    for (AgentInterface* agent: agents){
        if (pid == agent->getPid()){
            agent->sig_callback(child_status, terminated);
            break;
        }
    }
}

/* Child Signal Handler */
void sigchld_handler(int sig){
    int olderrno = errno;
    sigset_t mask_all, prev_all;
    pid_t pid;
    int child_status;

    sigfillset(&mask_all);
    while((pid = waitpid(-1, &child_status, WNOHANG)) > 0){
        //Block all signals while we handle this child
        sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        //Do stuff to process child signal
        remove_child(pid, child_status, false);
        //Ready to receive signals
        sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }
    errno = olderrno;
    return;
}

__sighandler_t Signal(int signum, __sighandler_t handler){
    struct sigaction action, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;

    if (sigaction(signum, &action, &old_action) < 0){
        cout << "Signal Handler attachment error: " << strerror(errno) << endl;
        exit(1);
    }
    return (old_action.sa_handler);
}
