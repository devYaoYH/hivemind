#include "GameInterface.h"
using namespace std;

GameInterface::GameInterface(): agents(nullptr), num_agents(0){

}

GameInterface::GameInterface(vector<AgentInterface*>* agents): agents(agents), num_agents(0){
    num_agents = agents->size();
}

ERR_CODES GameInterface::invoke_agent(int agentIdx, string& input, string&output){
    //If index out of bounds, return error
    if (agentIdx >= (int)agents->size()){
        return print_err(AGENT_OB);
    }
    //Get pointer to our agent from vector
    AgentInterface* agent = (*agents)[agentIdx];
    //Check if it is still running, otherwise, report killed
    if (!agent->running()){
        return print_err(AGENT_KILLED);
    }

    //Resume our agent
    kill(-(agent->getPid()), SIGCONT);
    waitpid(agent->getPid(), NULL, WCONTINUED);
    //Do IO
    *agent << input;
    *agent >> output;
    //Put our agent back to sleep for next round
    kill(-(agent->getPid()), SIGSTOP);
    waitpid(agent->getPid(), NULL, WUNTRACED);

    //Report if READ process has errors
    if (output.compare(ERROR) == 0){
        return print_err(RUNTIME_ERROR);
    }
    else if (output.compare(TIMEOUT) == 0){
        return print_err(AGENT_TIMEOUT);
    }
    return print_err(SUCCESS);
}

ERR_CODES GameInterface::print_err(ERR_CODES e){
    switch(e){
        case AGENT_OB:
            cout << "Agent ID out of bounds (Agent does not exist)" << endl;
            return e;
        case AGENT_KILLED:
            cout << "Agent TERMINATED" << endl;
            return e;
        case RUNTIME_ERROR:
            cout << "Agent ABORTED due to RUNTIME ERROR" << endl;
            return e;
        case AGENT_TIMEOUT:
            cout << "Agent TIMEOUT during execution" << endl;
            return e;
        default:
            return e;
    }
}

GameInterface::~GameInterface(){

}
