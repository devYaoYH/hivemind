#include "AgentInterface.h"
#define TMP_MS 1000000
using namespace std;

AgentInterface::AgentInterface(): meta_data(nullptr), is_auto(true), is_running(false){
    pipes[0] = 0;
    pipes[1] = 0;
}

map<string, string>* AgentInterface::getPlayerInfo(){
	return meta_data;
}

bool AgentInterface::isAuto(){
	return is_auto;
}

void AgentInterface::sig_callback(int status, bool state){
    is_running = state;
    child_status = status;
}

bool AgentInterface::running(){
    return is_running;
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
    agent.move(to_read);
    return agent;
}

//Default read from fd
void AgentInterface::move(string& output){
    fd_set input_pipe;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = TMP_MS;
    FD_ZERO(&input_pipe);
    FD_SET(pipes[STDIN], &input_pipe);
    output.clear();
    //cerr << "READING from: " << cmdline;
    if (is_auto){
        cerr << "READING from: " << cmdline;
        if (select(pipes[STDIN]+1, &input_pipe, NULL, NULL, &timeout)){
            char buf[2];
            while(read(pipes[STDIN], buf, 1) > 0 && buf[0] != '\n'){
                output += buf[0];
            }
            cerr << "Process used: " << TMP_MS - timeout.tv_usec << "us" << endl;
        }
        else{
            cerr << "ERROR! Read child process timeout: " << endl;
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
