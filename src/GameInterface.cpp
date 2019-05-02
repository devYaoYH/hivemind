#include "GameInterface.h"
using namespace std;

GameInterface::GameInterface(): agents() {
    
}

GameInterface::~GameInterface(){
    for (AgentInterface* agent: agents){
        delete agent;
    }
}
