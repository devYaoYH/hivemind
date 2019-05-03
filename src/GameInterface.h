#pragma once
#include "includes.h"
#include "AgentInterface.h"
using namespace std;

#ifndef GAME_INTERFACE
#define GAME_INTERFACE
enum ERR_CODES{
    SUCCESS,
    AGENT_OB,
    AGENT_KILLED,
    RUNTIME_ERROR,
    AGENT_TIMEOUT
};
#endif

class GameInterface {
	public:
		GameInterface();
        GameInterface(vector<AgentInterface*>* agents);
        virtual ~GameInterface();

        ERR_CODES invoke_agent(int agentIdx, string& input, string& output);
	protected:
		vector<AgentInterface*>* agents;
        int num_agents;
};
