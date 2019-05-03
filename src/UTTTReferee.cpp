#include "UTTTReferee.h"
using namespace std;

UTTTReferee::UTTTReferee(shared_ptr<GameInterface> game): Referee(){
    hive = game;
    last_move = "-1 -1";
}

void UTTTReferee::turn(){
    string init_output = last_move + "\n1\n0 0\n";
    hive->invoke_agent(0, init_output, last_move);
    cerr << last_move << endl;
    string init_output2 = last_move + "\n1\n0 0\n";
    hive->invoke_agent(1, init_output2, last_move);
    cerr << last_move << endl;
}

void UTTTReferee::run(){
    string init_output = last_move + "\n1\n0 0\n";
    hive->invoke_agent(0, init_output, last_move);
    cerr << last_move << endl;
    string init_output2 = last_move + "\n1\n0 0\n";
    hive->invoke_agent(1, init_output2, last_move);
    cerr << last_move << endl;
    for (int i=0;i<10;++i){
        turn();
    }
}

UTTTReferee::~UTTTReferee(){

}
