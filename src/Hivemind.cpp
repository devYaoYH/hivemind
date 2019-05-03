// Hivemind.cpp : Defines the entry point for the console application.
#include "includes.h"
#include "AgentBuilder.h"
#include "AgentInterface.h"
#include "GameInterface.h"
#include "Referee.h"
#include "SimpleReferee.h"

char* arg[MAX_ARGS];

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

    builder.getAgent("/usr/bin/python timed_py.py\n", agents);
    builder.getAgent("/usr/bin/java SimpleIO\n", agents);
    builder.getAgent("timed_child\n", agents);
    
    //Start GameInterface Object that handles process I/O
    GameInterface* game_handle = new GameInterface(&agents);

    //Create our Referee Object
    Referee* referee = new SimpleReferee(game_handle);
    
    //TODO: Waits till GameManager terminates
    referee->run();
    
    //Loop through agents and wait till execution terminates
    for (AgentInterface* agent: agents){
        if (agent->running()){
            kill(-(agent->getPid()), SIGKILL);
            while(agent->running()){
                continue;
            }
        }
    }
    cout << "Children closed successfully" << endl;

    //Only delete AgentInterface objects here
    for (AgentInterface* agent: agents) delete agent;

    return 0;
}

/* Process Methods */
void remove_child(pid_t pid, int child_status, bool running){
    char s[] = "child_terminated\n";
    write(STDOUT_FILENO, s, 17);
    for (AgentInterface* agent: agents){
        if (pid == agent->getPid()){
            agent->sig_callback(child_status, running);
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
