#pragma once
#include "TGrid.h"
using namespace std;

class SmallGrid: public TGrid{
    public:
        SmallGrid();
        virtual ~SmallGrid();

        int getPos(int r, int c);

        virtual int winner();
        virtual int play(pair<int, int> move, int player);
        virtual void getValidLocations(vector<pair<int, int> >& output_list);
    private:
        int grid[3][3];
};
