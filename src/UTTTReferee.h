#pragma once
#include "Referee.h"
#include "LargeGrid.h"
using namespace std;

class UTTTReferee: public Referee{
    public:
        UTTTReferee(shared_ptr<GameInterface> game);
        virtual ~UTTTReferee();

        virtual void run();
    private:
        int turn_count;
        int player_wins[2];
        pair<int, int> last_move;
        LargeGrid* board;
        bool turn();
        bool run_agent(int idx);
};
