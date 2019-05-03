#include "LargeGrid.h"
using namespace std;

LargeGrid::LargeGrid(): prev_move(pair<int, int>(-1, -1)){
    for (int r=0;r<3;++r){
        for (int c=0;c<3;++c){
            grid[r][c] = make_shared<SmallGrid>();
        }
    }
}

void LargeGrid::display(){
    int tmp_grid[9][9];
    for (int r=0;r<3;++r){
        for (int c=0;c<3;++c){
            //For each grid load pieces
            int gr = r*3, gc = c*3;
            for (int a=0;a<3;++a){
                for (int b=0;b<3;++b){
                    tmp_grid[gr+a][gc+b] = grid[r][c]->getPos(a, b);
                }
            }
        }
    }
    for (int r=0;r<9;++r){
        if (r%3 == 0) cerr << "-------------" << endl;
        for (int c=0;c<9;++c){
            if (c%3 == 0) cerr << '|';
            if (tmp_grid[r][c] == 0) cerr << 'o';
            else if (tmp_grid[r][c] == 1) cerr << 'x';
            else cerr << ' ';
        }
        cerr << '|' << endl;
    }
    cerr << "-------------" << endl;
}

int LargeGrid::winner(){
    bool has_moves = false;
    for (int r=0;r<3;++r){
        for (int c=0;c<3;++c){
            if (grid[r][c]->winner() == play_EMPTY){
                has_moves = true;
                break;
            }
        }
    }
    if (!has_moves) return play_DRAW;
    for (int i=0;i<8;++i){
        //Check each line
        int winner = grid[winning_lines[i][0][0]][winning_lines[i][0][1]]->winner();
        if (winner != 0 && winner != 1) continue;
        int line_winner = winner;
        for (int j=1;j<3;++j){
            int next_winner = grid[winning_lines[i][j][0]][winning_lines[i][j][1]]->winner();
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
    return play_EMPTY;
}

int LargeGrid::play(pair<int, int> move, int player){
    int gr = move.first/3, gc = move.second/3;
    //Check that player has played in a valid grid
    if (prev_move.first != -1 && grid[prev_move.first][prev_move.second]->winner() == play_EMPTY && (gr != prev_move.first || gc != prev_move.second)) return play_ERROR;
    int cr = move.first - gr*3, cc = move.second - gc*3;
    int play_result = grid[gr][gc]->play(pair<int, int>(cr, cc), player);
    if (play_result != play_ERROR){
        prev_move.first = cr;
        prev_move.second = cc;
    }
    return play_result;
}

void LargeGrid::getValidLocations(vector<pair<int, int> >& output_list){
    if (prev_move.first == -1 || grid[prev_move.first][prev_move.second]->winner() != play_EMPTY){
        //Can move anywhere
        for (int r=0;r<3;++r){
            for (int c=0;c<3;++c){
                if (grid[r][c]->winner() != play_EMPTY) continue;
                vector<pair<int, int> > tmp;
                grid[r][c]->getValidLocations(tmp);
                for (pair<int, int> cell: tmp){
                    output_list.push_back(pair<int, int>(r*3 + cell.first, c*3 + cell.second));
                }
            }
        }
    }
    else{
        //Restricted to previous grid ONLY
        vector<pair<int, int> > tmp;
        grid[prev_move.first][prev_move.second]->getValidLocations(tmp);
        for (pair<int, int> c: tmp){
            output_list.push_back(pair<int, int>(prev_move.first*3 + c.first, prev_move.second*3 + c.second));
        }
    }
    return;
}

LargeGrid::~LargeGrid(){

}
