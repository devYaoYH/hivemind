#pragma once
#include "includes.h"
#include "GameInterface.h"
using namespace std;

class Referee{
    public:
        //Interface Methods
        virtual void run() = 0;
    protected:
        GameInterface* hive;
};
