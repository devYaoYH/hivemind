#pragma once
#include "AgentInterface.h"
using namespace std;

//Concrete class for Robot Agent processes to Interface with
class RobotAgentInterface : public AgentInterface {
	public:
		RobotAgentInterface();				//Defaults to a Robot Agent
		virtual ~RobotAgentInterface();		//virtual destructor
		virtual string move();				//Function that our main referee calls to get a move
		virtual void update();				//Passes current game state information to Agent
};
