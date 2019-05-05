#pragma once
#include "includes.h"
#include "AgentInterface.h"
using namespace std;

//Builder class to help us bundle things together into an AgentInterface
//Spawns processes for our main program
class AgentBuilder{
    public:
        AgentBuilder();
        AgentBuilder(map<string, string>* game_config, vector<string>* agent_cmdlines);
        virtual ~AgentBuilder();
        
        bool genAgents(vector<AgentInterface*>& agents);
    private:
        char* arg[MAX_ARGS];
        static int num_agents;
        map<string, string>* game_config;
        vector<string>* agent_cmdlines;
        void parseline(const char* cmdline, char** argv);       //Cmdline parser from 361 shell-lab
        bool getAgent(string cmdline, vector<AgentInterface*>& agents);
};
