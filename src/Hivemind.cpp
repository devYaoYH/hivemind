// Hivemind.cpp : Defines the entry point for the console application.
#include "includes.h"
#include "Parser.h"
#include "AgentBuilder.h"
#include "AgentInterface.h"
#include "GameInterface.h"
#include "Referee.h"
#include "MyReferee.h"
using namespace std;

#ifndef NUM_AGENTS
#define NUM_AGENTS -1
#endif

#define BASE 10

//Common Declarations

//Keys for our Agent config
const string A_TYPE = "type";
const string A_CMD = "cmd";

//Keys for our Game config
const string C_TIME_INIT = "init";
const string C_TIME_ROUND = "round";
const string C_GAME_NUM = "games";

const string config_fname = "config.json";
bool debug_mode = false;
bool verbose_mode = false;
bool fair_mode = false;

char* arg[MAX_ARGS];
vector<AgentInterface*> agents;

//Keep agent stats across multiple games
int agent_stats[NUM_AGENTS][NUM_AGENTS+1];

//Default AgentBuilder Configurations
int time_init = 1000;
int time_round = 50;
int game_num = 1;

//Signal Handler
void sigchld_handler(int sig);
__sighandler_t Signal(int signum, __sighandler_t handler);

//Process Control Methods
void remove_child(pid_t pid, int child_status, bool terminated);

//Help Utility
void help(){
    cout << "Config Flags: -h -d -v -s" << endl;
    cout << "    -h: Congrats! You found out what -h does" << endl;
    cout << "    -d: Enable Debug Mode (prints lots of stuff to cout)" << endl;
    cout << "    -v: Visual Output Mode (tell Referee to print layout for console viewing)" << endl;
    cout << "    -s: Swap positions (every game is played twice, reversing movement order of agents for the second iteration)" << endl;
}

int main(int argc, char* argv[])
{
    //Get command line arguments for -v -d flags
    int c;
    while((c = getopt(argc, argv, "hvds")) != -1){
        switch(c){
            case 'h':
                help();
                return 0;
            case 'v':
                verbose_mode = true;
                break;
            case 'd':
                debug_mode = true;
                break;
            case 's':
                fair_mode = true;
                break;
        }
    }
    
    //Parse config file
    Parser* p_config = new Parser();
    
    //Parse our agents
    map<string, string> agents_config;
    vector<string> agent_names;
    vector<string> agent_cmdlines;
    p_config->getAttrs(agents_config, agent_names, "agents", config_fname);
    for (string s: agent_names){
        if (debug_mode) cout << s << endl;
        map<string, string> cur_agent_config;
        vector<string> tmp_agent_objs;
        string cmdline = "";
        p_config->getAttrs(cur_agent_config, tmp_agent_objs, s, config_fname);
        for (pair<string, string> kv: cur_agent_config){
            if (debug_mode) cout << "    " << kv.first << ": " << kv.second << endl;
            if (kv.first.compare(A_TYPE) == 0){
                cmdline = kv.second + " " + cmdline;
            }
            else if (kv.first.compare(A_CMD) == 0){
                cmdline += kv.second;
            }
        }
        cmdline += "\n";
        agent_cmdlines.push_back(cmdline);
    }

    //Parse gameManager configurations
    map<string, string> game_config;
    vector<string> game_config_objs;
    p_config->getAttrs(game_config, game_config_objs, "config", config_fname);
    if (debug_mode) cout << "GameManager Configurations Loaded: " << endl;
    for (pair<string, string> kv: game_config){
        if (debug_mode) cout << "    " << kv.first << ": " << kv.second << endl;
    }

    //Extract game settings if available
    char* tmp;
    if (game_config.find(C_TIME_INIT) != game_config.end()){
        int config_ti = strtol(game_config[C_TIME_INIT].c_str(), &tmp, BASE);
        if (config_ti > 0) time_init = config_ti;
    }
    if (game_config.find(C_TIME_ROUND) != game_config.end()){
        int config_tr = strtol(game_config[C_TIME_ROUND].c_str(), &tmp, BASE);
        if (config_tr > 0) time_round = config_tr;
    }
    if (game_config.find(C_GAME_NUM) != game_config.end()){
        int config_gn = strtol(game_config[C_GAME_NUM].c_str(), &tmp, BASE);
        if (config_gn > 0) game_num = config_gn;
    }

    //Exit on parsing errors
    if (NUM_AGENTS == -1){
        cout << "Error, NUM_AGENTS is not set!\nSet it within <Referee>.h" << endl;
        return 1;
    }
    if (agent_names.size() != NUM_AGENTS){
        cout << "Error, number of agents in configuration file: " << agent_names.size() << " is less than expected: " << NUM_AGENTS << endl;
        return 1;
    }
    
    //Install signal handler for child processes
    Signal(SIGCHLD, sigchld_handler);
    
    //Initiate a Builder class for us to generate Agent objects from config file
    AgentBuilder builder = AgentBuilder(&game_config, &agent_cmdlines);
    
    //Configure our builder
    builder.config(time_init, time_round);
    
    //Start GameInterface Object that handles process I/O
    shared_ptr<GameInterface> game_handle = make_shared<GameInterface>(&agents);

    //Redirect stderr to log file
    string log_file = "game.log";
    int file_err = open(log_file.c_str(), O_FLAGS, S_FLAGS);
    dup2(file_err, fileno(stderr));
    close(file_err);
    
    //Play for game_num Iterations
    for (int i=0;i<game_num;++i){
        cout << "Running Game: " << i+1 << endl;
        cerr << "=========" << endl;
        cerr << "| " << "GAME" << i+1 << " |" << endl;
        cerr << "=========" << endl;
        //Generate multiple agents and place them into a vector<AgentInterface*>
        if (!builder.genAgents(agents)){
            cout << "Error, unable to initialize Agents!" << endl;
            return 2;
        }
        else{
            cout << "Agents spawned successfully..." << endl;
        }
        
        //Create our Referee Object
        Referee* referee = new MyReferee(game_handle);
        
        //Blocking call Referee terminates
        int* results = referee->run();

        //Add to agent stats
        for (int j=0;j<NUM_AGENTS;++j){
            cout << results[j] << " ";
        }
        cout << endl;
        for (int j=0;j<NUM_AGENTS;++j){
            if (results[j] == -1) agent_stats[j][0]++;
            else agent_stats[j][results[j]]++;
        }

        /* CLEANUP */
        //Loop through agents and wait till execution terminates
        for (AgentInterface* agent: agents){
            if (agent->running()){
                kill(-(agent->getPid()), SIGKILL);
                //Busy Loop till we get successful termination of child process via interrupt by SIGCHLD
                while(agent->running()){
                    continue;
                }
            }
        }
        if (debug_mode) cout << "Children closed successfully" << endl;
        //Clean up heap resources
        for (AgentInterface* agent: agents) delete agent;
        agents.clear();
        delete referee;

        //Swap agent positions and play again
        if (fair_mode){
            cout << "Running Game: " << i+1 << " [reversed]" << endl;
            cerr << "=============" << endl;
            cerr << "| " << "GAME" << i+1 << " (r) |" << endl;
            cerr << "=============" << endl;
            //Generate multiple agents and place them into a vector<AgentInterface*>
            if (!builder.genAgents(agents)){
                cout << "Error, unable to initialize Agents!" << endl;
                return 2;
            }
            else{
                cout << "Agents spawned successfully..." << endl;
            }
            reverse(agents.begin(), agents.end());
            
            //Create our Referee Object
            Referee* referee = new MyReferee(game_handle);
            
            //Blocking call Referee terminates
            int* results = referee->run();

            //Add to agent stats
            for (int j=NUM_AGENTS-1;j>=0;--j){
                cout << results[j] << " ";
            }
            cout << endl;
            for (int j=0;j<NUM_AGENTS;++j){
                if (results[j] == -1) agent_stats[NUM_AGENTS-1-j][0]++;
                else agent_stats[NUM_AGENTS-1-j][results[j]]++;
            }

            /* CLEANUP */
            //Loop through agents and wait till execution terminates
            for (AgentInterface* agent: agents){
                if (agent->running()){
                    kill(-(agent->getPid()), SIGKILL);
                    //Busy Loop till we get successful termination of child process via interrupt by SIGCHLD
                    while(agent->running()){
                        continue;
                    }
                }
            }
            if (debug_mode) cout << "Children closed successfully" << endl;
            //Clean up heap resources
            for (AgentInterface* agent: agents) delete agent;
            agents.clear();
            delete referee;
        }
    }

    for (int i=0;i<NUM_AGENTS;++i){
        cout << "Agent " << i << " Stats:" << endl;
        for (int j=1;j<=NUM_AGENTS;++j){
            cout << "    Position " << j << ": " << agent_stats[i][j] << endl;
        }
        cout << "    CRASHED: " << agent_stats[i][0] << " times" << endl;
    }

    delete p_config;
    return 0;
}

/* Process Methods */
void set_agent_status(pid_t pid, int child_status, bool running, bool stopped){
    char tmp[] = "child reporting\n";
    char run_stat = '0' + running;
    char stop_stat = '0' + stopped;
    char debug_str[] = {run_stat, '|', stop_stat};
    if (debug_mode) write(STDOUT_FILENO, tmp, 16);
    if (debug_mode) write(STDOUT_FILENO, debug_str, 1);
    if (debug_mode) write(STDOUT_FILENO, debug_str+1, 1);
    if (debug_mode) write(STDOUT_FILENO, debug_str+2, 1);
    for (AgentInterface* agent: agents){
        if (pid == agent->getPid()){
            agent->sig_callback(child_status);
            agent->set_running(running);
            agent->set_stopped(stopped);
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
    while((pid = waitpid(-1, &child_status, WNOHANG | WUNTRACED | WCONTINUED)) > 0){
        //Block all signals while we handle this child
        sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        //Do stuff to process child signal
        if (WIFEXITED(child_status) || WIFSIGNALED(child_status)){
            set_agent_status(pid, child_status, false, false);
        }
        else if (WIFSTOPPED(child_status)){
            set_agent_status(pid, child_status, true, true);
        }
        else if (WIFCONTINUED(child_status)){
            set_agent_status(pid, child_status, true, false);
        }
        else{
            char err[] = "child uncaught\n";
            if (debug_mode) write(STDOUT_FILENO, err, 15);
        }
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
