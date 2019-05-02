#include "AgentInterface.h"
#define TMP_MS 1000000
using namespace std;

AgentInterface::AgentInterface(): meta_data(nullptr), is_auto(true) {
    pipes[0] = 0;
    pipes[1] = 0;
}

map<string, string>* AgentInterface::getPlayerInfo() {
	return meta_data;
}

bool AgentInterface::isAuto() {
	return is_auto;
}

// Specifies the which fd we should READ from
// and which we should WRITE to
void AgentInterface::attach_pipes(int fd_in, int fd_out){
    pipes[STDIN] = fd_in;
    pipes[STDOUT] = fd_out;
}

//Writes to Agent with string obj specified
AgentInterface& operator<<(AgentInterface& agent, string& to_write){
    cout << "Writing to Agent: " << to_write << endl;
    agent.update(to_write);
    return agent;
}

//Reads from Agent to reference of string obj
AgentInterface& operator>>(AgentInterface& agent, string& to_read){
    cout << "Reading from Agent: " << to_read << endl;
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
    if (select(pipes[STDIN]+1, &input_pipe, NULL, NULL, &timeout)){
        char buf[2];
        output.clear();
        while(read(pipes[STDIN], buf, 1) > 0 && buf[0] != '\n'){
            output += buf[0];
        }
        cerr << "Time left: " << timeout.tv_usec << "us | I/O overhead: " << TMP_MS - 50000 - timeout.tv_usec << "us" << endl;
    }
    else{
        cerr << "ERROR! Read child process timeout: " << endl;
    }
}

//Default write to fd
void AgentInterface::update(string& input){
    write(pipes[STDOUT], input.c_str(), input.length());
}

void AgentInterface::load_meta(const string keySeq, const string fname) {
	Parser* config_parser = new Parser();
	vector<string> tmp_objs;
	config_parser->getAttrs(*meta_data, tmp_objs, keySeq, fname);
}

AgentInterface::~AgentInterface() {
	delete meta_data;
}
