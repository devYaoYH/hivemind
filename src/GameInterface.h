#pragma once
#include "includes.h"
#include "AgentInterface.h"
using namespace std;

class GameInterface {
	public:
		GameInterface(vector<AgentInterface*> agents);
        virtual ~GameInterface();
        
        //Virtual Methods to be implemented
        virtual void run() = 0;                             //Main Blocking loop
	protected:
		vector<AgentInterface*> agents;
};
