#pragma once
#include "Referee.h"
using namespace std;

class UTTTReferee: public Referee{
    public:
        UTTTReferee(shared_ptr<GameInterface> game);
        virtual ~UTTTReferee();

        virtual void run();
    private:
        void turn();
        string last_move;
};
