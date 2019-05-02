#pragma once
#include "includes.h"
#include "GameInterface.h"
using namespace std;

class SimpleReferee: public GameInterface{
    public:
        SimpleReferee();
        virtual ~SimpleReferee();
        
        virtual void agentMove(int agentIdx, string move);
        virtual void outputState(int agentIdx);
};
