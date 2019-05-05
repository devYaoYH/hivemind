#pragma once
#include "includes.h"
#include "AgentInterface.h"
using namespace std;

class GameInterface {
	public:
		GameInterface();
        GameInterface(vector<AgentInterface*>* agents);
        virtual ~GameInterface();

        ERR_CODES invoke_agent(int agentIdx, string& input, string& output);
	protected:
		vector<AgentInterface*>* agents;
        int num_agents;
        ERR_CODES print_err(ERR_CODES e);
};
