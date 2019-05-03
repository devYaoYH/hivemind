#pragma once
#include "includes.h"
using namespace std;

#ifndef TGRID_CONSTS
#define TGRID_CONSTS
const int play_EMPTY = -1;
const int play_ERROR = -2;
const int play_DRAW = -3;
#endif

class TGrid{
    public:
        TGrid();
        virtual ~TGrid();

        //Report to game referee who won
        virtual int winner() = 0;
        //Simulate a move on the board
        virtual int play(pair<int, int> move, int player) = 0;
        //Get a list of playable locations
        virtual void getValidLocations(vector<pair<int, int> >& output_list) = 0;
    protected:
        static int winning_lines[8][3][2];
};
