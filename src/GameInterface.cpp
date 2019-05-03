#include "GameInterface.h"
using namespace std;

GameInterface::GameInterface(): agents(nullptr), num_agents(0){

}

GameInterface::GameInterface(vector<AgentInterface*>* agents): agents(agents), num_agents(0){
    num_agents = agents->size();
}

ERR_CODES GameInterface::invoke_agent(int agentIdx, string& input, string&output){
    //If index out of bounds, return error
    if (agentIdx >= num_agents) return AGENT_OB;
    //Get pointer to our agent from vector
    AgentInterface* agent = (*agents)[agentIdx];
    //Check if it is still running, otherwise, report killed
    if (!agent->running()) return AGENT_KILLED;

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
    if (output.compare(ERROR) == 0) return RUNTIME_ERROR;
    else if (output.compare(TIMEOUT) == 0) return AGENT_TIMEOUT;
    return SUCCESS;
}

GameInterface::~GameInterface(){

}
