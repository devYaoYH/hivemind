#include "SimpleReferee.h"
using namespace std;

//Initialize with agent vector
SimpleReferee::SimpleReferee(shared_ptr<GameInterface> game): Referee(){
    hive = game;
}

void SimpleReferee::turn(){
    //Dummy Interaction with child script
    string response;
    for (int i=0;i<3;++i){
        hive->invoke_agent(i, test_msg, response);
    }
}

//Referee Script
void SimpleReferee::run(){
    for(int i=0;i<10;++i) turn();
}

SimpleReferee::~SimpleReferee(){

}
