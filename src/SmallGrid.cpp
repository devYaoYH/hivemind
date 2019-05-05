#include "SmallGrid.h"
using namespace std;

SmallGrid::SmallGrid(){
    for (int r=0;r<3;++r){
        for (int c=0;c<3;++c){
            grid[r][c] = -1;
        }
    }
}

int SmallGrid::getPos(int r, int c){
    return grid[r][c];
}

int SmallGrid::winner(){
    for (int i=0;i<8;++i){
        //Check each line
        int winner = grid[winning_lines[i][0][0]][winning_lines[i][0][1]];
        if (winner != 0 && winner != 1) continue;
        int line_winner = winner;
        for (int j=1;j<3;++j){
            int next_winner = grid[winning_lines[i][j][0]][winning_lines[i][j][1]];
            if (next_winner != winner){
                line_winner = -1;
                break;
            }
        }
        if (line_winner != -1){
            //We have a winner!!!
            return line_winner;
        }
    }
    bool has_moves = false;
    for (int r=0;r<3;++r){
        for (int c=0;c<3;++c){
            if (grid[r][c] == -1){
                has_moves = true;
                break;
            }
        }
    }
    if (!has_moves) return play_DRAW;
    return play_EMPTY;
}

int SmallGrid::play(pair<int, int> move, int player){
    if (grid[move.first][move.second] == -1){
        grid[move.first][move.second] = player;
    }
    else return play_ERROR;
    return winner();
}

void SmallGrid::getValidLocations(vector<pair<int, int> >& output_list){
    for (int r=0;r<3;++r){
        for (int c=0;c<3;++c){
            if (grid[r][c] == -1){
                output_list.push_back(pair<int, int>(r, c));
            }
        }
    }
    return;
}

SmallGrid::~SmallGrid(){
    
}
