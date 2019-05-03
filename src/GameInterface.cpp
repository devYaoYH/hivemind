#include "GameInterface.h"
using namespace std;

GameInterface::GameInterface(): agents(nullptr), num_agents(0){

}

GameInterface::GameInterface(vector<AgentInterface*>* agents): agents(agents), num_agents(0){
    num_agents = agents->size();
}

int GameInterface::invoke_agent(int agentIdx, string& input, string&output){
    if (agentIdx >= num_agents) return 1;
    AgentInterface* agent = (*agents)[agentIdx];
    kill(-(agent->getPid()), SIGCONT);
    waitpid(agent->getPid(), NULL, WCONTINUED);
    *agent << input;
    *agent >> output;
    kill(-(agent->getPid()), SIGSTOP);
    waitpid(agent->getPid(), NULL, WUNTRACED);
    return 0;
}

GameInterface::~GameInterface(){

}
