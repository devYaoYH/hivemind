#pragma once
#include "includes.h"
#include "AgentInterface.h"
using namespace std;

class GameInterface {
	public:
		GameInterface();
		virtual ~GameInterface();

		// Pass an Agent's move to the Referee
		virtual void agentMove(int agentIdx, string move) = 0;

		// Query for Game State output to pass to Agent
		virtual void outputState(int agentIdx, ostream& agent_stdin) = 0;
	protected:
		vector<AgentInterface> agents;
};