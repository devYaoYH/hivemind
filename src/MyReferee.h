#pragma once
#include "Referee.h"
#include "LargeGrid.h"
using namespace std;

/* Define how many agents our Referee expects */
#define NUM_AGENTS 2

class MyReferee: public Referee{
    public:
        MyReferee(shared_ptr<GameInterface> game);
        virtual ~MyReferee();

        virtual void run();
    private:
        int turn_count;
        int player_wins[NUM_AGENTS];
        pair<int, int> last_move;
        LargeGrid* board;
        bool turn();
        bool run_agent(int idx);
};
