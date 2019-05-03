#pragma once
#include "includes.h"
#include "AgentInterface.h"
using namespace std;

class GameInterface {
	public:
		GameInterface();
        GameInterface(vector<AgentInterface*>* agents);
        virtual ~GameInterface();

        int invoke_agent(int agentIdx, string& input, string& output);
	protected:
		vector<AgentInterface*>* agents;
        int num_agents;
};
