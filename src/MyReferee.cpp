#include "MyReferee.h"
using namespace std;

MyReferee::MyReferee(shared_ptr<GameInterface> game): Referee(), turn_count(0){
    hive = game;
    for (int i=0;i<NUM_AGENTS;++i){
        player_wins[i] = 0;
        player_rank[i] = 0;
    }
    last_move = pair<int, int>(-1, -1);
    board = new LargeGrid();
}

/* Referee Helpers */
bool MyReferee::run_agent(int idx){
    //Query the board with last move to get list of valid locations
    vector<pair<int, int> > next_locations;
    board->getValidLocations(next_locations);
    //Format it into a string to write to agent
    string to_child = to_string(last_move.first) + " " + to_string(last_move.second) + "\n" + to_string(next_locations.size()) + "\n";
    for (pair<int, int> p: next_locations){
        to_child += to_string(p.first) + " " + to_string(p.second) + "\n";
    }
    //Interact with agent to get response
    string response;
    ERR_CODES agent_status = hive->invoke_agent(idx, to_child, response);
    if (agent_status == SUCCESS){
        stringstream ss(response);
        int a, b;
        if (ss >> a && ss >> b){
            last_move.first = a;
            last_move.second = b;
            //Check outcome of this move
            int outcome = board->play(last_move, idx);
            if (outcome == idx){
                //Add to win boards
                player_wins[idx]++;
            }
            else if (outcome == play_ERROR){
                //Player made an invalid move
                cerr << "Agent " << idx << " Made an INVALID move: " << response << endl;
                return false;
            }

            //Only case where valid execution happens
            return true;
        }
        else{
            //Error badly formatted response
            cerr << "Agent " << idx << " Responded with bad string: " << response << endl;
        }
    }
    else{
        cerr << "Agent " << idx << " Responded with ERROR CODE: " << ERR_STRINGS[agent_status] << endl;
    }
    return false;
}

bool MyReferee::turn(){
    turn_count++;
    //Run agents sequentially
    for (int i=0;i<2;++i){
        if (!run_agent(i)){
            //Agent reports error (crashes)
            player_wins[i] = -1;
        }
        board->display();
        int play_result = board->winner();
        if (play_result == i){
            //We have a winner!!
            if (i == 0){
                player_rank[0] = 1;
                player_rank[1] = 2;
            }
            else{
                player_rank[0] = 2;
                player_rank[1] = 1;
            }
            return false;
        }
        else if (play_result == play_DRAW){
            //Draw game
            //Check how many small grids each player won
            if (player_wins[0] > player_wins[1]){
                //Player 0 wins!
                player_rank[0] = 1;
                player_rank[1] = 2;
            }
            else if (player_wins[1] > player_wins[0]){
                //Player 1 wins!
                player_rank[0] = 2;
                player_rank[1] = 1;
            }
            else{
                //DRAW GAME
                player_rank[0] = 1;
                player_rank[1] = 1;
            }
            return false;
        }
    }
    if (player_wins[0] == -1 && player_wins[1] == -1){
        //SAD, both players crashed
        //DRAW
        player_rank[0] = -1;
        player_rank[1] = -1;
        return false;
    }
    else if (player_wins[0] == -1){
        //Player 1 wins
        player_rank[0] = -1;
        player_rank[1] = 1;
        return false;
    }
    else if (player_wins[1] == -1){
        //Player 0 wins
        player_rank[0] = 1;
        player_rank[1] = -1;
        return false;
    }
    return true;
}

void MyReferee::print_result(){
    cerr << "#####" << endl;
    cerr << player_rank[0] << " " << player_rank[1] << endl;
}

int* MyReferee::run(){
    //Write initial move to agent
    do{
        cerr << "=====" << endl << turn_count << endl;
    } while(turn());
    board->display();
    print_result();
    return player_rank;
}

MyReferee::~MyReferee(){
    delete board;
}
