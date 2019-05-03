#pragma once
#include "TGrid.h"
#include "SmallGrid.h"
using namespace std;

class LargeGrid: public TGrid{
    public:
        LargeGrid();
        virtual ~LargeGrid();
        
        //Prints board to stderr
        void display();
        
        virtual int winner();
        virtual int play(pair<int, int> move, int player);
        virtual void getValidLocations(vector<pair<int, int> >& output_list);
    private:
        //Previously made move -> determines next grid
        pair<int, int> prev_move;
        //Grid of Tic Tac Toe objects
        shared_ptr<SmallGrid> grid[3][3];
};
