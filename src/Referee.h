#pragma once
#include "includes.h"
#include "GameInterface.h"
using namespace std;

class Referee{
    public:
        Referee() {}
        virtual ~Referee() {}
        //Interface Methods
        virtual int* run() = 0;
    protected:
        shared_ptr<GameInterface> hive;
};
