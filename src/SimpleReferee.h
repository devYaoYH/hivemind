#pragma once
#include "GameInterface.h"
using namespace std;

class SimpleReferee: public GameInterface{
    public:
        SimpleReferee(vector<AgentInterface*> agents);
        virtual ~SimpleReferee();

        virtual void run();
    private:
        void turn();
        void quit();
        string test_msg = "hey there\n";
        string end = "quit\n";
        string msg = "hello world\n";
};
