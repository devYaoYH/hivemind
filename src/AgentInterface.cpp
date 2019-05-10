#include "AgentInterface.h"
#define INIT_US 1001000
#define ROUND_US 51000
using namespace std;

AgentInterface::AgentInterface(): meta_data(nullptr), is_auto(true), is_running(false), is_stopped(false), has_init(false), child_status(0), t_init(INIT_US), t_round(ROUND_US){
    pipes[0] = 0;
    pipes[1] = 0;
}

AgentInterface::AgentInterface(int t_init, int t_round): meta_data(nullptr), is_auto(true), is_running(false), is_stopped(false), has_init(false), child_status(0), t_init(t_init*1000), t_round(t_round*1000){
    pipes[0] = 0;
    pipes[1] = 0;
}

map<string, string>* AgentInterface::getPlayerInfo(){
	return meta_data;
}

bool AgentInterface::isAuto(){
	return is_auto;
}

void AgentInterface::sig_callback(int status){
    child_status = status;
}

void AgentInterface::set_running(bool is_run){
    is_running = is_run;
}

bool AgentInterface::running(){
    return is_running;
}

void AgentInterface::set_stopped(bool is_stop){
    is_stopped = is_stop;
}

bool AgentInterface::stopped(){
    return is_stopped;
}

int AgentInterface::getChldStat(){
    return child_status;
}

void AgentInterface::setPid(pid_t pid){
    child_pid = pid;
}

pid_t AgentInterface::getPid(){
    return child_pid;
}

void AgentInterface::setCmd(string cmdline){
    this->cmdline = cmdline;
}

string AgentInterface::getCmd(){
    return cmdline;
}

// Specifies the which fd we should READ from
// and which we should WRITE to
void AgentInterface::attach_pipes(int fd_in, int fd_out){
    pipes[STDIN] = fd_in;
    pipes[STDOUT] = fd_out;
}

const int* AgentInterface::getFd(){
    return pipes;
}

//Writes to Agent with string obj specified
AgentInterface& operator<<(AgentInterface& agent, string& to_write){
    agent.update(to_write);
    return agent;
}

//Reads from Agent to reference of string obj
AgentInterface& operator>>(AgentInterface& agent, string& to_read){
    if (!agent.running()){
        to_read = string(ERROR.begin(), ERROR.end());
        return agent;
    }
    agent.move(to_read);
    return agent;
}

//Default read from fd
void AgentInterface::move(string& output){
    fd_set input_pipe;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = has_init?t_round:t_init;
    FD_ZERO(&input_pipe);
    FD_SET(pipes[STDIN], &input_pipe);
    output.clear();
    //cerr << "READING from: " << cmdline;
    if (is_auto){
        if (debug_mode) cout << "READING from: " << cmdline;
        if (select(pipes[STDIN]+1, &input_pipe, NULL, NULL, &timeout)){
            char buf[2];
            while(read(pipes[STDIN], buf, 1) > 0 && buf[0] != '\n'){
                output += buf[0];
            }
            if (debug_mode) cout << "Process used: " << (has_init?t_round:t_init) - timeout.tv_usec << "us" << endl;
            has_init = true;
        }
        else{
            //cerr << "ERROR! Read child process timeout: " << endl;
            output = string(TIMEOUT.begin(), TIMEOUT.end());
        }
    }
    else{
        //If this is a Human Agent, we block till we receive an input
        while(output.length() < 1){
            if (select(pipes[STDIN]+1, &input_pipe, NULL, NULL, &timeout)){
                char buf[2];
                while(read(pipes[STDIN], buf, 1) > 0 && buf[0] != '\n'){
                    output += buf[0];
                }
            }
        }
    }
}

//Default write to fd
void AgentInterface::update(string& input){
    write(pipes[STDOUT], input.c_str(), input.length());
}

void AgentInterface::load_meta(const string keySeq, const string fname) {
	if (meta_data == nullptr){
        meta_data = new map<string, string>();
    }
    Parser* config_parser = new Parser();
	vector<string> tmp_objs;
	config_parser->getAttrs(*meta_data, tmp_objs, keySeq, fname);
}

AgentInterface::~AgentInterface() {
	delete meta_data;
}
