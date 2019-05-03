#pragma once
#include "includes.h"
#include "GameInterface.h"
using namespace std;

class Referee{
    public:
        Referee() {}
        virtual ~Referee() {}
        //Interface Methods
        virtual void run() = 0;
    protected:
        shared_ptr<GameInterface> hive;
};
