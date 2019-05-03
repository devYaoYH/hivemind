#pragma once
#include "Referee.h"
using namespace std;

class SimpleReferee: public Referee{
    public:
        SimpleReferee(GameInterface* game);
        virtual ~SimpleReferee();

        virtual void run();
    private:
        void turn();
        string test_msg = "hey there\n";
        string end = "quit\n";
        string msg = "hello world\n";
};
